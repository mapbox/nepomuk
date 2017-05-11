#ifndef NEPOMUK_TOOL_CONTAINER_ERRORS_HPP_
#define NEPOMUK_TOOL_CONTAINER_ERRORS_HPP_

#include <stdexcept>

namespace nepomuk
{
namespace tool
{
namespace container
{

// Error Handling for Containers
struct FatalError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

// when size constraints are not fulfilled
struct SizeError : FatalError
{
    using FatalError::FatalError;
};

} // namespace container
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_CONTAINER_ERRORS_HPP_
