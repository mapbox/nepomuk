#ifndef NEPOMUK_TOOL_STATUS_PROGRESS_HPP_
#define NEPOMUK_TOOL_STATUS_PROGRESS_HPP_

#include "tool/status/timing.hpp"
#include <cstdint>
#include <string>

namespace nepomuk
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

class ProgressBarGuard
{
  public:
    ProgressBarGuard(std::uint32_t hundred_percent);
    ~ProgressBarGuard();

    void print(std::uint32_t current);

  private:
    std::uint32_t hundred_percent;
    std::uint8_t last;
};

} // namespace statue
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_STATUS_PROGRESS_HPP_
