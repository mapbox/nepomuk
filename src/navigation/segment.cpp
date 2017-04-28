#include "navigation/segment.hpp"

#include <boost/assert.hpp>
#include <cmath>

namespace transit
{
namespace navigation
{

namespace segment
{

///////////////////////////////////////////////////////////
date::UTCTimestamp Transit::departure() const
{
    BOOST_ASSERT(!_connections.empty());
    return _connections.front().departure();
}
date::UTCTimestamp Transit::arrival() const
{
    BOOST_ASSERT(!_connections.empty());
    return _connections.front().arrival();
}
std::uint32_t Transit::duration() const { return arrival() - departure(); }

///////////////////////////////////////////////////////////
date::UTCTimestamp Transfer::departure() const { return _departure; }
date::UTCTimestamp Transfer::arrival() const { return _arrival; }
std::uint32_t Transfer::duration() const { return _arrival - _departure; }
StopID Transfer::origin() const { return _origin; }
StopID Transfer::destination() const { return _destination; }

///////////////////////////////////////////////////////////
date::UTCTimestamp Walk::departure() const { return _departure; }
date::UTCTimestamp Walk::arrival() const { return _arrival; }
std::uint32_t Walk::duration() const { return _arrival - _departure; }
std::uint32_t Walk::distance() const
{
    return static_cast<std::uint32_t>(std::round(geometric::distance(_origin, _destination)));
}

geometric::WGS84Coordinate Walk::origin() const { return _origin; }
geometric::WGS84Coordinate Walk::destination() const { return _destination; }

} // namespace segment

void TimestampExtractorVisitor::operator()(segment::Walk const &segment)
{
    departure = segment.departure();
    arrival = segment.arrival();
}
void TimestampExtractorVisitor::operator()(segment::Transit const &segment)
{
    departure = segment.departure();
    arrival = segment.arrival();
}
void TimestampExtractorVisitor::operator()(segment::Transfer const &segment)
{
    departure = segment.departure();
    arrival = segment.arrival();
}

bool Segment::is_transit() const { return _type == SegmentType::TRANSIT; }
bool Segment::is_transfer() const { return _type == SegmentType::TRANSFER; }
bool Segment::is_walk() const { return _type == SegmentType::WALK; }

SegmentType Segment::type() const { return _type; }

segment::Transfer const &Segment::as_transfer() const
{
    BOOST_ASSERT(is_transfer());
    return boost::get<segment::Transfer>(container);
}

segment::Transfer &Segment::as_transfer()
{
    BOOST_ASSERT(is_transfer());
    return boost::get<segment::Transfer>(container);
}

segment::Transit const &Segment::as_transit() const
{
    BOOST_ASSERT(is_transit());
    return boost::get<segment::Transit>(container);
}

segment::Transit &Segment::as_transit()
{
    BOOST_ASSERT(is_transit());
    return boost::get<segment::Transit>(container);
}

segment::Walk const &Segment::as_walk() const
{
    BOOST_ASSERT(is_walk());
    return boost::get<segment::Walk>(container);
}

segment::Walk &Segment::as_walk()
{
    BOOST_ASSERT(is_walk());
    return boost::get<segment::Walk>(container);
}

date::UTCTimestamp Segment::departure() const
{
    TimestampExtractorVisitor visitor;
    visit(std::move(visitor));
    return visitor.departure;
}

date::UTCTimestamp Segment::arrival() const
{
    TimestampExtractorVisitor visitor;
    visitor = visit(std::move(visitor));
    return visitor.arrival;
}

} // namespace navigation
} // namespace transit
