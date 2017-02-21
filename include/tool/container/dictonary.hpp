#ifndef TRANSIT_TOOL_CONTAINER_DICTONARY_HPP_
#define TRANSIT_TOOL_CONTAINER_DICTONARY_HPP_

#include <vector>

namespace transit
{
namespace tool
{
namespace container
{

template <typename data_type> class Dictonary : public std::vector<data_type>
{
    /* for now this is only an extension to the vector, after we can look at implementing a
     * reasonable structure
     * optimised for string views */
};

} // namespace container
} // namespace container
} // namespace transit

#endif // TRANSIT_TOOL_CONTAINER_DICTONARY_HPP_
