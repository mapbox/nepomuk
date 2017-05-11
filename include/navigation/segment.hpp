#ifndef NEPOMUK_NAVIGATION_SEGMENT_HPP_
#define NEPOMUK_NAVIGATION_SEGMENT_HPP_

#include <boost/assert.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/variant/variant.hpp>

#include <cstdint>
#include <vector>

#include "date/time.hpp"
#include "geometric/coordinate.hpp"
#include "id/stop.hpp"

#include "navigation/connection.hpp"
#include "navigation/stop.hpp"

namespace nepomuk
{
namespace navigation
{

class RoutingAlgorithm;

namespace segment
{

// the connection between two main stops (including intermediate stops) along a line
class Transit
{
  private:
    // the stops along the segment
    std::vector<Stop> _stops;
    std::vector<Connection> _connections;

    // make sure routing algorithms are allowed to construct segments
    friend class ::nepomuk::navigation::RoutingAlgorithm;

  public:
    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;
    std::uint32_t duration() const;

    auto stops() const { return boost::make_iterator_range(_stops.begin(), _stops.end()); }
    auto connections() const
    {
        return boost::make_iterator_range(_connections.begin(), _connections.end());
    }
};

// transfer segment between to lines
class Transfer
{
  public:
    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;
    std::uint32_t duration() const;

    StopID origin() const;
    StopID destination() const;

  private:
    StopID _origin;
    StopID _destination;

    date::UTCTimestamp _departure;
    date::UTCTimestamp _arrival;

    // make sure routing algorithms are allowed to construct segments
    friend class ::nepomuk::navigation::RoutingAlgorithm;
};

// a walking segment, usually in the begin/end of a route
class Walk
{
  public:
    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;
    std::uint32_t duration() const;
    std::uint32_t distance() const;

    geometric::WGS84Coordinate origin() const;
    geometric::WGS84Coordinate destination() const;

  private:
    geometric::WGS84Coordinate _origin;
    geometric::WGS84Coordinate _destination;

    date::UTCTimestamp _departure;
    date::UTCTimestamp _arrival;
    // make sure routing algorithms are allowed to construct segments
    friend class ::nepomuk::navigation::RoutingAlgorithm;
};

} // namespace segment

// within a trip, a segment describes a single segment between two distinct transfers. A segment can
// be a
// single connection (e.g. take the 100 from A to B), or a series of possible connections.
// We can immagine taking any of a series of busses, depending on which one we manage to catch in
// case of close connections. In that case we would offer take any of 100/101/231 from A to B
using SegmentContainer = boost::variant<segment::Transfer, segment::Transit, segment::Walk>;

enum class SegmentType
{
    TRANSIT,
    TRANSFER,
    WALK
};

class Segment
{
  public:
    bool is_transit() const;
    bool is_transfer() const;
    bool is_walk() const;

    SegmentType type() const;

    segment::Transfer const &as_transfer() const;
    segment::Transit const &as_transit() const;
    segment::Walk const &as_walk() const;

    segment::Transfer &as_transfer();
    segment::Transit &as_transit();
    segment::Walk &as_walk();

    template <typename visitor_type> visitor_type visit(visitor_type visitor) const;

    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;

  private:
    SegmentType _type;
    SegmentContainer container;

    friend class RoutingAlgorithm;
};

struct TimestampExtractorVisitor
{
    void operator()(segment::Walk const &segment);
    void operator()(segment::Transit const &segment);
    void operator()(segment::Transfer const &segment);

    date::UTCTimestamp departure;
    date::UTCTimestamp arrival;
};

// visit the segment, according to it's type
template <typename visitor_type> visitor_type Segment::visit(visitor_type visitor) const
{
    switch (type())
    {
    case SegmentType::TRANSIT:
        visitor(as_transit());
        return visitor;
    case SegmentType::TRANSFER:
        visitor(as_transfer());
        return visitor;
    default:
        BOOST_ASSERT(type() == SegmentType::WALK);
        visitor(as_walk());
        return visitor;
    }
}

} // namespace navigation
} // namespace nepomuk

#endif // NEPOMUK_NAVIGATION_SEGMENT_HPP_
