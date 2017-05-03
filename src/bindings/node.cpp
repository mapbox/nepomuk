#include "bindings/node.hpp"

#include "service/earliest_arrival.hpp"
#include "service/tile.hpp"

#include "date/time.hpp"

#include <cmath>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>

#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>

namespace transit
{
namespace binding
{

namespace
{

template <typename target_type>
boost::optional<target_type> extract_optional(v8::Local<v8::Value> const &value)
{
    return boost::none;
}

template <> boost::optional<bool> extract_optional(v8::Local<v8::Value> const &value)
{
    if (!value->IsBoolean())
        throw std::runtime_error("Expecting boolean value.");
    return {value->BooleanValue()};
}

template <> boost::optional<double> extract_optional(v8::Local<v8::Value> const &value)
{
    if (!value->IsNumber())
        throw std::runtime_error("Expecting number.");
    return {value->NumberValue()};
}

template <> boost::optional<std::uint32_t> extract_optional(v8::Local<v8::Value> const &value)
{
    if (!value->IsUint32())
        throw std::runtime_error("Expecting number.");
    return {static_cast<std::uint32_t>(Nan::To<std::uint32_t>(value).FromJust())};
}

template <typename target_type>
boost::optional<target_type> get_optional(std::string const &option_name,
                                          v8::Local<v8::Object> const &object)
{
    auto const nan_option_name = Nan::New(option_name.c_str()).ToLocalChecked();
    if (!object->Has(nan_option_name))
        return boost::none;
    else
    {
        try
        {
            auto const as_local = Nan::Get(object, nan_option_name).ToLocalChecked();
            if (as_local->IsUndefined())
                return boost::none;
            return extract_optional<target_type>(as_local);
        }
        catch (std::runtime_error error)
        {
            throw std::runtime_error(option_name + ": " + error.what());
        }
    }
};

service::TileParameters adaptTileParameters(v8::Local<v8::Object> const &object)
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

std::vector<geometric::WGS84Coordinate> parse_coordinates(v8::Local<v8::Array> const &object)
{
    std::vector<geometric::WGS84Coordinate> result;
    if (object.IsEmpty())
        throw std::runtime_error(
            "Failed to request coordinates. Did you specify correct coordinates?");

    if (object->IsUndefined())
        throw std::runtime_error("Must provide a coordinates property");

    if (!object->IsArray())
        throw std::runtime_error("Coordinates has to be an array of lon/lat pairs.");

    for (std::uint32_t object_index = 0; object_index < object->Length(); ++object_index)
    {
        v8::Local<v8::Value> coordinate = object->Get(object_index);
        if (coordinate.IsEmpty())
            throw std::runtime_error("Found empty coordinate set");

        if (!coordinate->IsArray())
            throw std::runtime_error("Coordinates must be an array of (lon/lat) pairs");

        v8::Local<v8::Array> coordinate_pair = v8::Local<v8::Array>::Cast(coordinate);
        if (coordinate_pair->Length() != 2)
            throw std::runtime_error("Coordinates must be an array of (lon/lat) pairs");

        if (!coordinate_pair->Get(0)->IsNumber() || !coordinate_pair->Get(1)->IsNumber())
            throw std::runtime_error("Each member of a coordinate pair must be a number");

        double lon = coordinate_pair->Get(0)->NumberValue();
        double lat = coordinate_pair->Get(1)->NumberValue();

        if (std::isnan(lon) || std::isnan(lat) || std::isinf(lon) || std::isinf(lat))
            throw std::runtime_error("Lon/Lat coordinates must be valid numbers");

        if (lon > 180 || lon < -180 || lat > geometric::constants::EPSG3857_MAX_LATITUDE ||
            lat < -geometric::constants::EPSG3857_MAX_LATITUDE)
            throw std::runtime_error("Lon/Lat coordinates must be within WGS84 bounds "
                                     "(-180 < lng < 180, -85.05 < lat < 85.05)");

        result.emplace_back(geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
                            geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat));
    }
    return result;
}

service::EarliestArrivalParameters adaptEAPParameters(v8::Local<v8::Object> const &value)
{
    // needs to be an object
    if (!value->IsObject())
        throw std::runtime_error("Earliest arrival requires an object as parameter.");

    auto const object = Nan::To<v8::Object>(value).ToLocalChecked();
    // requires three parameters, date, departure, and coordinates. Coordinates has to be an array
    // of two coordinates (from/to)
    v8::Local<v8::Value> v8_coordinates = object->Get(Nan::New("coordinates").ToLocalChecked());
    auto const coordinates = parse_coordinates(v8::Local<v8::Array>::Cast(v8_coordinates));

    if (coordinates.size() != 2)
        throw std::runtime_error("Earliest Arrival Queries need to specify exactly two locations");

    auto const optional_radius = get_optional<double>("walking_radius", object);
    auto const optional_speed = get_optional<double>("walking_speed", object);
    auto const optional_departure = get_optional<std::uint32_t>("departure", object);
    auto const optional_arrival = get_optional<std::uint32_t>("arrival", object);
    auto const optional_transfer_scale = get_optional<std::uint32_t>("transfer_scale", object);

    if (optional_arrival && optional_departure)
        throw std::runtime_error("Cannot specify both departure and arrival time.");

    if (!optional_departure && !optional_arrival)
        throw std::runtime_error("Missing one of departure/arrival");

    auto const walking_radius = optional_radius ? *optional_radius : 1000.0;
    auto const walking_speed = optional_speed ? *optional_speed : 1.0;
    auto const transfer_scale = optional_transfer_scale ? *optional_transfer_scale : 1.0;
    boost::optional<date::UTCTimestamp> arrival =
        optional_arrival ? boost::optional<date::UTCTimestamp>{*optional_arrival} : boost::none;
    boost::optional<date::UTCTimestamp> departure =
        optional_departure ? boost::optional<date::UTCTimestamp>{*optional_departure} : boost::none;

    return service::EarliestArrivalParameters(coordinates[0],
                                              coordinates[1],
                                              departure,
                                              arrival,
                                              walking_radius,
                                              walking_speed,
                                              transfer_scale);
}

service::ServiceParameters get_parameters(std::string const &plugin_identifier,
                                          v8::Local<v8::Object> const &object)
{
    if (plugin_identifier == "tile")
        return {service::PluginType::TILE, adaptTileParameters(object)};
    else if (plugin_identifier == "eap")
        return {service::PluginType::EAP, adaptEAPParameters(object)};
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

void Worker::HandleOKCallback() try
{
    Nan::HandleScope scope;

    auto const extract_tile_result = [](auto const &service_parameters) {
        auto const &tile_parameters =
            boost::get<service::TileParameters>(service_parameters.parameters);
        return static_cast<std::string>(tile_parameters.result());
    };

    auto const extract_eap_result = [](auto const &service_parameters) {
        auto const &earliest_arrival_parameters =
            boost::get<service::EarliestArrivalParameters>(service_parameters.parameters);
        return earliest_arrival_parameters.result();
    };

    auto const service_result_as_string = [&]() {
        switch (service_parameters.type)
        {
        case service::PluginType::TILE:
            return extract_tile_result(service_parameters);
        case service::PluginType::EAP:
            return extract_eap_result(service_parameters);
        }
    }();

    auto result = Nan::New(std::cref(service_result_as_string)).ToLocalChecked();
    /*
    auto result = Nan::CopyBuffer(service_result_as_string.data(), service_result_as_string.size())
                      .ToLocalChecked();
    */

    const auto argc = 2u;
    v8::Local<v8::Value> argv[argc] = {Nan::Null(), std::move(result)};

    callback->Call(argc, argv);
}
catch (const std::exception &e)
{
    std::cout << "[error] failed request due to: " << e.what() << std::endl;
    return Nan::ThrowError(e.what());
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
    std::cout << "[error] failed request due to: " << e.what() << std::endl;
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
    else if (plugin_name == "eap")
    {
        auto *const self = Nan::ObjectWrap::Unwrap<Engine>(info.Holder());
        self->master_service->register_plugin(
            "eap", std::make_shared<service::EarliestArrival>(*self->master_service));
    }
    else
    {
        throw std::runtime_error("Plugin [" + plugin_name + "] not implemented.");
    }
}
catch (const std::exception &e)
{
    std::cout << "[error] failed request due to: " << e.what() << std::endl;
    return Nan::ThrowError(e.what());
}

// do an asynchronous request against the master service with a set of parameters
NAN_METHOD(Engine::request) try
{
    if (info.Length() < 3)
        throw std::runtime_error("Parameter missmatch: Requiring plugin name, plugin "
                                 "parameters and callback.");

    if (!info[0]->IsString())
        throw std::runtime_error("First parameter needs to be the plugin name.");

    auto const plugin_name = *v8::String::Utf8Value(Nan::To<v8::String>(info[0]).ToLocalChecked());

    auto *const self = Nan::ObjectWrap::Unwrap<Engine>(info.Holder());
    auto service = self->master_service->get(plugin_name);

    if (!service)
        throw std::runtime_error("Requesting plugin that hasn't been plugged in. "
                                 "Register the plugin first.");

    if (!info[2]->IsFunction())
        throw std::runtime_error("Last parameter needs to be the callback");

    auto *worker = new Worker{self->master_service,
                              std::move(service),
                              get_parameters(plugin_name, info[1].As<v8::Object>()),
                              new Nan::Callback{info[2].As<v8::Function>()}};
    Nan::AsyncQueueWorker(worker);
}
catch (const std::exception &e)
{
    std::cout << "[error] failed request due to: " << e.what() << std::endl;
    return Nan::ThrowError(e.what());
}

} // namespace binding
} // namespace transit
