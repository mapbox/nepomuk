#ifndef TRANSIT_TOOL_TIMING_HPP_
#define TRANSIT_TOOL_TIMING_HPP_

#include <chrono>
#include <cstdint>

namespace transit
{
namespace tool
{
namespace status
{
// timer class that starts automatically or manually (start)
class Timer
{
  public:
    Timer() : _start(std::chrono::steady_clock::now()) {}
    void start() { _start = std::chrono::steady_clock::now(); }
    void stop() { _stop = std::chrono::steady_clock::now(); }

    std::int64_t nanoseconds()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(_stop - _start).count();
    }
    std::int64_t microseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(_stop - _start).count();
    }
    std::int64_t milliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_stop - _start).count();
    }
    std::int64_t seconds()
    {
        return std::chrono::duration_cast<std::chrono::seconds>(_stop - _start).count();
    }
    std::int64_t minutes()
    {
        return std::chrono::duration_cast<std::chrono::minutes>(_stop - _start).count();
    }

  private:
    std::chrono::time_point<std::chrono::steady_clock> _start;
    std::chrono::time_point<std::chrono::steady_clock> _stop;
};

} // namespace status
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_TIMING_HPP_
