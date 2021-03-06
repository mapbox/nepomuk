#include "timetable/station_table.hpp"
#include "geometric/coordinate.hpp"
#include "timetable/exceptions.hpp"
#include "timetable/station_table_factory.hpp"

#include "gtfs/stop.hpp"
#include "id/dictionary.hpp"
#include "id/stop.hpp"
#include <boost/optional.hpp>
#include <cstdint>

using namespace nepomuk;
using namespace nepomuk::timetable;
using namespace nepomuk::gtfs;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

Stop makeStop(std::uint64_t station, std::uint64_t parent)
{
    if (parent != station)
        return {StopID{station},
                DictionaryID{0},
                geometric::WGS84Coordinate(
                    geometric::makeLatLonFromDouble<geometric::FixedLongitude>(0),
                    geometric::makeLatLonFromDouble<geometric::FixedLatitude>(0)),
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
                DictionaryID{0},
                geometric::WGS84Coordinate(
                    geometric::makeLatLonFromDouble<geometric::FixedLongitude>(0),
                    geometric::makeLatLonFromDouble<geometric::FixedLatitude>(0)),
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
    auto table = StationTableFactory::produce(data.begin(), data.end());
    BOOST_CHECK_EQUAL(table.num_stations(), 2);
    BOOST_CHECK_EQUAL(table.num_stops(), 2);
}
