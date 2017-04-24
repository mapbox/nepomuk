#include "tool/status/progress.hpp"
#include <iostream>

namespace transit
{
namespace tool
{
namespace status
{

FunctionTimingGuard::FunctionTimingGuard(std::string identifier_)
    : identifier(std::move(identifier_))
{
    std::cout << "[" << identifier << "] started..." << std::endl;
}

FunctionTimingGuard::~FunctionTimingGuard()
{
    timer.stop();
    std::cout << "[" << identifier << "] done: " << timer.milliseconds() << " ms. || "
              << timer.seconds() << " s" << std::endl;
}

ProgressBarGuard::ProgressBarGuard(std::uint32_t hundred_percent)
    : hundred_percent(hundred_percent), last(0)
{
}

ProgressBarGuard::~ProgressBarGuard() { std::cout << " done." << std::endl; }

void ProgressBarGuard::print(std::uint32_t current)
{
    auto const val = 100 * current / hundred_percent;
    if (val != last)
    {
        if (val % 10 == 0)
            std::cout << " " << val << "% " << std::flush;
        else if (val % 2 == 0)
            std::cout << "." << std::flush;
        last = static_cast<std::uint8_t>(val);
    }
}

} // namespace statue
} // namespace tool
} // namespace transit
