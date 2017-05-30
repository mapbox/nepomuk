#ifndef NEPOMUK_BINDING_WORKERS_HPP_
#define NEPOMUK_BINDING_WORKERS_HPP_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <nan.h>
#pragma GCC diagnostic pop
#include "ipc/request_handler.hpp"

#include "adaptor/ipc_response_to_json.hpp"

#include <exception>
#include <string>

namespace nepomuk
{
namespace binding
{

class Worker : public Nan::AsyncWorker
{
    using Base = Nan::AsyncWorker;

  public:
    Worker(std::string const &socket_id, Nan::Callback *callback);

    void HandleOKCallback() override;

  protected:
    ipc::RequestHandler request_handler;
    std::string json_result;
};

class ShutdownWorker : public Worker
{
    using Base = Worker;

  public:
    ShutdownWorker(std::string const &socket_id, Nan::Callback *callback);
    void Execute() override;
};

// the poor worker that has to carry all the load
template <typename task_parameters> class TaskWorker final : public Worker
{
    using Base = Worker;

  public:
    TaskWorker(std::string const &socket_id, task_parameters parameters, Nan::Callback *callback);

    void Execute() override;

  private:
    // to ensure the datasets stay valid
    task_parameters parameters;
};

template <typename task_parameters>
TaskWorker<task_parameters>::TaskWorker(std::string const &socket_id,
                                        task_parameters parameters,
                                        Nan::Callback *callback)
    : Worker(socket_id, callback), parameters(parameters)
{
}

template <typename task_parameters> void TaskWorker<task_parameters>::Execute() try
{
    auto response = request_handler(parameters);
    adaptor::IPCResponseToJSON adaptor;
    Base::json_result = adaptor(response);
}
catch (const std::exception &e)
{
    SetErrorMessage(e.what());
}

} // namespace binding
} // namespace nepomuk

#endif // NEPOMUK_BINDING_WORKERS_HPP_
