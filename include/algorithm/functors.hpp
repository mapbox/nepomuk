#ifndef TRANSIT_ALGORITHM_FUNCTORS_HPP_
#define TRANSIT_ALGORITHM_FUNCTORS_HPP_

namespace transit
{
namespace algorithm
{
// add a specific value to an element
template <typename value_type, value_type> value_type increase_by(value_type &value);

template <typename value_type, value_type by> value_type increase_by(value_type &value)
{
    value += by;
    return value;
}

} // namespace algorithm
} // namespace transit

#endif // TRANSIT_ALGORITHM_FUNCTORS_HPP_
