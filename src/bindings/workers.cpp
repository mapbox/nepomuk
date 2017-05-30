#include "bindings/workers.hpp"

namespace nepomuk
{
namespace binding
{

Worker::Worker(std::string const &socket_id, Nan::Callback *callback)
    : Base(callback), request_handler(socket_id)
{
}

void Worker::HandleOKCallback()
{
    Nan::HandleScope scope;

    auto result = Nan::New(std::cref(json_result)).ToLocalChecked();

    const auto argc = 2u;
    v8::Local<v8::Value> argv[argc] = {Nan::Null(), std::move(result)};

    callback->Call(argc, argv);
}

ShutdownWorker::ShutdownWorker(std::string const &socket_id, Nan::Callback *callback)
    : Base(socket_id, callback)
{
}

void ShutdownWorker::Execute() { request_handler.send_shutdown(); }

} // namespace binding
} // namespace nepomuk
