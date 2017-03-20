#ifndef TRANSIT_TOOL_STATUS_PROGRESS_HPP_
#define TRANSIT_TOOL_STATUS_PROGRESS_HPP_

#include "tool/status/timing.hpp"
#include <string>

namespace transit
{
namespace tool
{
namespace status
{

struct FunctionTimingGuard
{
    FunctionTimingGuard(std::string identifier);
    ~FunctionTimingGuard();

    std::string identifier;
    Timer timer;
};

} // namespace statue
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_STATUS_PROGRESS_HPP_
