#include "annotation/line.hpp"

namespace nepomuk
{
namespace annotation
{

LineInfo const &Line::operator()(LineID const line) const
{
    BOOST_ASSERT(line.base() < line_information.size());
    return line_information[line.base()];
}
}
} // namespace nepomuk
