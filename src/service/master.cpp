#include "service/master.hpp"

#include "geometric/coordinate.hpp"
#include "gtfs/read_csv.hpp"
#include "id/stop.hpp"

#include "adaptor/dictionary.hpp"
#include "annotation/geometry_factory.hpp"
#include "annotation/stop_info.hpp"
#include "annotation/trip.hpp"
#include "search/stop_to_line_factory.hpp"
#include "timetable/graph_adaptor.hpp"
#include "timetable/timetable_factory.hpp"

#include "tool/status/progress.hpp"

#include <boost/assert.hpp>
#include <utility>
#include <vector>

namespace transit
{
namespace service
{

Master::Master(std::string const &path)
    : base_data(gtfs::readCSV(gtfs::CSVDiscSource(path.c_str())))
{
    // the constructor only reads the base data. All other classes / look-ups are constructed on
    // demand
    base_data.filter_unreachable_stops();
    base_data.connect_stops_into_stations(100);
}

// request the plugin from the master service
std::shared_ptr<service::Interface> Master::get(std::string const &identifier)
{
    auto const itr = plugins.find(identifier);

    return itr != plugins.end() ? itr->second : nullptr;
}

// register an interface. If the identifier did exist beforehand, the old plugin is returned
std::shared_ptr<service::Interface>
Master::register_plugin(std::string const &identifier,
                        std::shared_ptr<service::Interface> interface)
{
    auto const current_itr = plugins.find(identifier);
    auto current = current_itr != plugins.end() ? current_itr->second : nullptr;
    plugins[identifier] = std::move(interface);
    return current;
}

// remove a service from the plugin architecture, returns the plugin that was in ther service if
// one existed
std::shared_ptr<service::Interface> Master::deregister(std::string const &identifier)
{
    auto const current_itr = plugins.find(identifier);
    auto current = current_itr != plugins.end() ? current_itr->second : nullptr;

    plugins.erase(identifier);
    return current;
}

// routing data
timetable::TimeTable const &Master::timetable()
{
    if (!_timetable)
    {
        tool::status::FunctionTimingGuard guard("Timetable creation");
        shape_from_line = std::make_unique<std::vector<boost::optional<ShapeID>>>();
        _timetable = std::make_unique<timetable::TimeTable>(
            timetable::TimeTableFactory::produce(base_data, *shape_from_line));
    }

    return *_timetable;
}

// look-ups
search::StopToLine const &Master::stop_to_line()
{
    if (!_stop_to_line)
    {
        tool::status::FunctionTimingGuard guard("Stop To Line Lookup-creation");
        _stop_to_line = std::make_unique<search::StopToLine>(
            search::StopToLineFactory::produce(base_data.stops.size(), timetable()));
    }

    return *_stop_to_line;
}

search::CoordinateToStop const &Master::coordinate_to_stop()
{
    if (!_coordinate_to_stop)
    {
        tool::status::FunctionTimingGuard guard("Coordinate to Stop Lookup-creation");
        auto make_coordinate_lookup = [&]() {
            std::vector<std::pair<geometric::WGS84Coordinate, StopID>> data;
            std::for_each(base_data.stops.begin(), base_data.stops.end(), [&](auto const &element) {
                data.push_back(std::make_pair(element.location, element.id));
            });
            return search::CoordinateToStop(data);
        };

        _coordinate_to_stop = std::make_unique<search::CoordinateToStop>(make_coordinate_lookup());
    }
    return *_coordinate_to_stop;
}

// plain data
tool::container::StringTable const &Master::dictionary()
{
    if (!_dictionary)
    {
        tool::status::FunctionTimingGuard guard("Dictionary Creation");
        auto const message = adaptor::Dictionary::encode(base_data.dictionary);
        _dictionary = std::make_unique<tool::container::StringTable>();
        adaptor::Dictionary::decode_into(*_dictionary, message);
        // release the full dictionary, we don't need it anymore
        base_data.dictionary = {};
    }
    return *_dictionary;
}

timetable::SegmentTable const &Master::segment_table()
{
    if (!_segment_table)
    {
        tool::status::FunctionTimingGuard guard("Segment Table Creation");
        _segment_table =
            std::make_unique<timetable::SegmentTable>(base_data.shapes_as_indexed_vector());
    }
    return *_segment_table;
}

// annotation
annotation::StopInfoTable const &Master::stop_info_annotation()
{
    if (!_stop_info_annotation)
    {
        tool::status::FunctionTimingGuard guard("Stop Info Annotation creation");
        auto const &ttable = timetable();
        BOOST_ASSERT(shape_from_line);
        _stop_info_annotation = std::make_unique<annotation::StopInfoTable>(base_data.stops);
    }

    return *_stop_info_annotation;
}

annotation::Geometry const &Master::geometry_annotation()
{
    if (!_geometry_annotation)
    {
        tool::status::FunctionTimingGuard guard("Geometry Annotation - creation");
        // required to get shape_from_line
        auto const &ttable = timetable();
        (void)ttable;
        _geometry_annotation = std::make_unique<annotation::Geometry>(
            annotation::GeometryFactory::produce(base_data.stops,
                                                 *shape_from_line,
                                                 stop_to_line(),
                                                 timetable().lines(),
                                                 segment_table()));
    }
    return *_geometry_annotation;
}

annotation::Trip const &Master::trip_annotation()
{
    if (!_trip_annotation)
    {
        tool::status::FunctionTimingGuard guard("Trip Annotation - creation");
        _trip_annotation = std::make_unique<annotation::Trip>(
            stop_info_annotation(), geometry_annotation(), dictionary());
    }

    return *_trip_annotation;
}

annotation::OSRM const &Master::osrm_annotation()
{
    if (!_osrm_annotation)
    {
        tool::status::FunctionTimingGuard guard("OSRM Annotation - creation");
        _osrm_annotation = std::make_unique<annotation::OSRM>(
            stop_info_annotation(), dictionary(), geometry_annotation());
    }

    return *_osrm_annotation;
}

algorithm::StronglyConnectedComponent const &Master::components()
{
    if (!_components)
    {
        tool::status::FunctionTimingGuard guard("Components creation");
        auto graph = timetable::TimetableToGraphAdaptor::adapt(timetable(), stop_to_line());
        _components =
            std::make_unique<algorithm::StronglyConnectedComponent>(algorithm::computeSCC(graph));
    }
    return *_components;
}

} // namespace service
} // namespace transit
