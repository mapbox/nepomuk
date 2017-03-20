#ifndef TRANSIT_TOOL_CONTAINER_ERRORS_HPP_
#define TRANSIT_TOOL_CONTAINER_ERRORS_HPP_

#include <stdexcept>

namespace transit
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

struct ConstructionError : FatalError
{
    using FatalError::FatalError;
};

} // namespace container
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_CONTAINER_ERRORS_HPP_
