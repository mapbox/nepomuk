#include "timetable/station_table.hpp"
#include "geometric/coordinate.hpp"
#include "timetable/exceptions.hpp"
#include "timetable/station_table_factory.hpp"
#include "tool/container/dictionary.hpp"

#include "gtfs/stop.hpp"
#include <boost/optional.hpp>
#include <cstdint>

using namespace transit::timetable;
using namespace transit::gtfs;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

Stop makeStop(std::uint64_t station, std::uint64_t parent)
{
    if (parent != station)
        return {StopID{station},
                transit::tool::container::DictionaryID{0},
                transit::geometric::Coordinate(
                    transit::geometric::makeLatLonFromDouble<transit::geometric::FixedLongitude>(0),
                    transit::geometric::makeLatLonFromDouble<transit::geometric::FixedLatitude>(0)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                StopID{parent},
                boost::none,
                boost::none};
    else
        return {StopID{station},
                transit::tool::container::DictionaryID{0},
                transit::geometric::Coordinate(
                    transit::geometric::makeLatLonFromDouble<transit::geometric::FixedLongitude>(0),
                    transit::geometric::makeLatLonFromDouble<transit::geometric::FixedLatitude>(0)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none};
}

BOOST_AUTO_TEST_CASE(check_input_validity)
{
    // cannot use empty vectors
    std::vector<Stop> data;
    data.push_back(makeStop(0, 1));
    BOOST_CHECK_THROW(StationTableFactory::produce(data.begin(), data.end()), InvalidInputError);
    data.push_back(makeStop(1, 1));
    BOOST_CHECK_NO_THROW(StationTableFactory::produce(data.begin(), data.end()));
}
