#ifndef NEPOMUK_ALGORITHM_FUNCTORS_HPP_
#define NEPOMUK_ALGORITHM_FUNCTORS_HPP_

namespace nepomuk
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
} // namespace nepomuk

#endif // NEPOMUK_ALGORITHM_FUNCTORS_HPP_
