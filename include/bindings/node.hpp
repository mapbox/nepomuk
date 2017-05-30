#ifndef NEPOMUK_BINDINGS_NODE_HPP_
#define NEPOMUK_BINDINGS_NODE_HPP_

#include <memory>
#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <nan.h>
#pragma GCC diagnostic pop
#include "service/route_parameters.hpp"
#include "service/tile_parameters.hpp"

namespace nepomuk
{
namespace binding
{

// This struct provides a node wrapper for the transit service. This allows for querying a server
// for transit routes in their various forms and the debug map. Every service provided by the engine
// needs to be set-up here.
// 
// The class itself is deriving from a wrapped object using the NAN library.
class Engine final : public Nan::ObjectWrap
{
    using Base = Nan::ObjectWrap;

  public:
    // basic set-up of the engine
    // static void init(v8::Local<v8::Object>, v8::Local<v8::Object> module);
    static NAN_MODULE_INIT(init);

    // the registered services
    // static void request(const Nan::FunctionCallbackInfo<v8::Value> &);
    static NAN_METHOD(request);

    // the registered services
    // static void request(const Nan::FunctionCallbackInfo<v8::Value> &);
    static NAN_METHOD(send_shutdown);

  private:
    // static void create(const Nan::FunctionCallbackInfo<v8::Value> &);
    static NAN_METHOD(create);

    Engine(std::string const &socket_id);
    Engine(Engine &&) = delete;
    Engine &operator=(Engine &&) = delete;

    // Since the engine for the node bindings is constructed from java script land, we need to
    // ensure the consistency / construction calling ourselves.
    // The following function guarantees singleton access into the class.
    static Nan::Persistent<v8::Function> &construct();

  private:
    std::string socket_id;
};

} // namespace binding
} // namespace nepomuk

// export the module into the global node namespace
NODE_MODULE(nepomuk, nepomuk::binding::Engine::init)

#endif // NEPOMUK_BINDINGS_NODE_HPP_
