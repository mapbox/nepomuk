#ifndef NEPOMUK_IPC_CONTEXT_HPP_
#define NEPOMUK_IPC_CONTEXT_HPP_

#include <zmq.hpp>

namespace nepomuk
{
namespace ipc
{

// singleton for creating a global zero_mq context
static zmq::context_t &get_context()
{
    static zmq::context_t context(2);
    return context;
}

} // namespace ipc
} // namespace nepomuk

#endif // NEPOMUK_IPC_CONTEXT_HPP_
