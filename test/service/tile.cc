// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "gtfs/dataset.hpp"
#include "gtfs/read_csv.hpp"
#include "gtfs/time.hpp"

#include "search/coordinate_to_stop.hpp"
#include "search/stop_to_line_factory.hpp"
#include "service/tile.hpp"
#include "service/tile_parameters.hpp"
#include "timetable/timetable_factory.hpp"

#include <iterator>

using namespace transit;

BOOST_AUTO_TEST_CASE(render_tiles)
{
    // This needs to be replaced by a dedicated fixture (see
    // https://github.com/mapbox/directions-transit/issues/37)
    gtfs::CSVDiscSource source(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    auto dataset = gtfs::readCSV(source);

    auto const timetable = timetable::TimeTableFactory::produce(dataset);
    auto const trip_look_up = search::StopToLineFactory::produce(dataset.stops.size(), timetable);

    auto make_coordinate_lookup = [&]() {
        std::vector<std::pair<geometric::Coordinate, gtfs::StopID>> data;
        std::for_each(dataset.stops.begin(), dataset.stops.end(), [&](auto const &element) {
            if (!element.location_type || *element.location_type == gtfs::LocationType::STOP)
                data.push_back(std::make_pair(element.location, element.id));
        });
        return search::CoordinateToStop(data);
    };

    auto coordinate_lookup = make_coordinate_lookup();

    service::Tile tileservice(timetable, coordinate_lookup, trip_look_up);
}
