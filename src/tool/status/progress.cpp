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

} // namespace statue
} // namespace tool
} // namespace transit
