#include "bindings/node.hpp"

#include "service/tile.hpp"

#include <functional>
#include <stdexcept>

namespace transit
{
namespace binding
{

namespace
{
service::TileParameters adaptTileParameters(v8::Local<v8::Value> const object)
{
    if (!object->IsArray())
        throw std::runtime_error("Parameter must be an array [z, x, y]");

    v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(object);

    if (array->Length() != 3)
        throw std::runtime_error("Parameter must be an array [z, x, y]");

    v8::Local<v8::Value> z = array->Get(0);
    v8::Local<v8::Value> x = array->Get(1);
    v8::Local<v8::Value> y = array->Get(2);

    if (!x->IsUint32() && !x->IsUndefined())
        throw std::runtime_error("Tile x coordinate must be unsigned interger");

    if (!y->IsUint32() && !y->IsUndefined())
        throw std::runtime_error("Tile y coordinate must be unsigned interger");

    if (!z->IsUint32() && !z->IsUndefined())
        throw std::runtime_error("Tile z coordinate must be unsigned interger");

    service::TileParameters params(Nan::To<std::uint32_t>(x).FromJust(),
                                   Nan::To<std::uint32_t>(y).FromJust(),
                                   Nan::To<std::uint32_t>(z).FromJust());

    if (!params.valid())
        throw std::runtime_error("Invalid tile coordinates");

    return params;
}

service::TileParameters get_parameters(std::string const &plugin_identifier,
                                       v8::Local<v8::Value> const &object)
{
    if (plugin_identifier == "tile")
        return {adaptTileParameters(object)};
    else
        throw std::runtime_error("Plugin parameters are not supported.");
};

} // namespace

Worker::Worker(std::shared_ptr<service::Master> master_service,
               std::shared_ptr<service::Interface> service,
               service::ServiceParameters service_parameters,
               Nan::Callback *callback)
    : Base(callback), master_service(std::move(master_service)), service(std::move(service)),
      service_parameters(std::move(service_parameters))
{
}

void Worker::Execute()
{
    auto const result = (*service)(service_parameters);

    if (result != service::ServiceStatus::SUCCESS)
    {
        // TODO this should become a error string based on the type of error / some internal status
        SetErrorMessage("Failed to fulfill request");
    }
}

void Worker::HandleOKCallback()
{
    Nan::HandleScope scope;

    auto const &tile_parameters = boost::get<service::TileParameters>(service_parameters);
    auto const as_string = static_cast<std::string>(tile_parameters.result());

    auto result = Nan::CopyBuffer(as_string.data(), as_string.size()).ToLocalChecked();

    const auto argc = 2u;
    v8::Local<v8::Value> argv[argc] = {Nan::Null(), std::move(result)};

    callback->Call(argc, argv);
}

Engine::Engine(std::string const &path) : master_service(std::make_shared<service::Master>(path)) {}

Nan::Persistent<v8::Function> &Engine::construct()
{
    static Nan::Persistent<v8::Function> init;
    return init;
}

NAN_MODULE_INIT(Engine::init)
{
    auto const whoami = Nan::New("Engine").ToLocalChecked();

    auto fnTp = Nan::New<v8::FunctionTemplate>(create);
    fnTp->InstanceTemplate()->SetInternalFieldCount(1);
    fnTp->SetClassName(whoami);

    SetPrototypeMethod(fnTp, "plug", plug);
    SetPrototypeMethod(fnTp, "request", request);

    auto const fn = Nan::GetFunction(fnTp).ToLocalChecked();

    construct().Reset(fn);

    // target stems from the node_module_init
    Nan::Set(target, whoami, fn);
}

// initialise the internal data structures
NAN_METHOD(Engine::create) try
{
    // Handle `new T()` as well as `T()`
    if (!info.IsConstructCall())
    {
        auto init = Nan::New(construct());
        info.GetReturnValue().Set(init->NewInstance());
        return;
    }

    if (info.Length() != 1)
        throw std::runtime_error("Create requires a single parameter (to gtfs dataset)");

    if (!info[0]->IsString())
        throw std::runtime_error("Parameter to create needs to be a single string.");

    auto *self = new Engine(*v8::String::Utf8Value(Nan::To<v8::String>(info[0]).ToLocalChecked()));
    self->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}
catch (const std::exception &e)
{
    return Nan::ThrowError(e.what());
}

NAN_METHOD(Engine::plug) try
{
    if (info.Length() != 1)
        throw std::runtime_error("Create requires a single parameter (to gtfs dataset)");

    if (!info[0]->IsString())
        throw std::runtime_error("Parameter to create needs to be a single string.");

    std::string plugin_name = *v8::String::Utf8Value(Nan::To<v8::String>(info[0]).ToLocalChecked());

    if (plugin_name == "tile")
    {
        // plug in a new service
        auto *const self = Nan::ObjectWrap::Unwrap<Engine>(info.Holder());
        self->master_service->register_plugin(
            "tile", std::make_shared<service::Tile>(*self->master_service));
    }
    else
    {
        throw std::runtime_error("Plugin [" + plugin_name + "] not implemented.");
    }
}
catch (const std::exception &e)
{
    return Nan::ThrowError(e.what());
}

// do an asynchronous request against the master service with a set of parameters
NAN_METHOD(Engine::request) try
{
    if (info.Length() < 3)
        throw std::runtime_error(
            "Parameter missmatch: Requiring plugin name, plugin parameters and callback.");

    if (!info[0]->IsString())
        throw std::runtime_error("First parameter needs to be the plugin name.");

    v8::Local<v8::Object> plugin_name_object = Nan::To<v8::Object>(info[0]).ToLocalChecked();
    auto const plugin_name =
        *v8::String::Utf8Value(Nan::To<v8::String>(plugin_name_object).ToLocalChecked());

    auto *const self = Nan::ObjectWrap::Unwrap<Engine>(info.Holder());
    auto service = self->master_service->get(plugin_name);

    if (!service)
        throw std::runtime_error(
            "Requesting plugin that hasn't been plugged in. Register the plugin first.");

    if (!info[2]->IsFunction())
        throw std::runtime_error("Last parameter needs to be the callback");

    auto *worker = new Worker{self->master_service,
                              std::move(service),
                              get_parameters(plugin_name, info[1]),
                              new Nan::Callback{info[2].As<v8::Function>()}};
    Nan::AsyncQueueWorker(worker);
}
catch (const std::exception &e)
{
    return Nan::ThrowError(e.what());
}

} // namespace binding
} // namespace transit
