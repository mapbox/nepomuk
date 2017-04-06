#ifndef TRANSIT_SERVICE_MASTER_HPP_
#define TRANSIT_SERVICE_MASTER_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "gtfs/dataset.hpp"
#include "service/interface.hpp"

// the basic timetable
#include "timetable/segment_table.hpp"
#include "timetable/timetable.hpp"

// look-ups
#include "search/coordinate_to_stop.hpp"
#include "search/stop_to_line.hpp"

// pure data
#include "tool/container/string_table.hpp"

// annotation
#include "annotation/stop_info.hpp"
#include "annotation/trip.hpp"

// graph measures
#include "algorithm/scc.hpp"

namespace transit
{
namespace service
{

// The master service is a meta-service that simply delegates tasks to the respective sub-services.
// The main reason for having it is to ensure that all data remains active, when we do asynchronous
// calls, without having to realy on shared pointers all over the code. Services can be registered,
// as long as they follow the interface characteristic.
class Master
{
  public:
    Master(std::string const &path);

    // request the plugin from the master service
    std::shared_ptr<service::Interface> get(std::string const &identifier);

    // register an interface. If the identifer did exist beforehand, the old plugin is returned
    std::shared_ptr<service::Interface>
    register_plugin(std::string const &identifier, std::shared_ptr<service::Interface> interface);

    // remove a service from the plugin architecture, returns the plugin that was in ther service if
    // one existed
    std::shared_ptr<service::Interface> deregister(std::string const &identifier);

    // routing data
    timetable::TimeTable const &timetable();

    // look-ups
    search::StopToLine const &stop_to_line();
    search::CoordinateToStop const &coordinate_to_stop();

    // plain data
    tool::container::StringTable const &dictionary();
    timetable::SegmentTable const &segment_table();

    // annotation
    annotation::Trip const &trip_annotation();
    annotation::StopInfoTable const &stop_info_annotation();

    // graph measurements
    algorithm::StronglyConnectedComponent const &components();

  private:
    gtfs::Dataset base_data;
    std::unique_ptr<std::vector<boost::optional<ShapeID>>> shape_from_line;

    // timetable
    std::unique_ptr<timetable::TimeTable> _timetable;

    // look-ups
    std::unique_ptr<search::StopToLine> _stop_to_line;
    std::unique_ptr<search::CoordinateToStop> _coordinate_to_stop;

    // plain data
    std::unique_ptr<tool::container::StringTable> _dictionary;
    std::unique_ptr<timetable::SegmentTable> _segment_table;

    // annotation
    std::unique_ptr<annotation::Trip> _trip_annotation;
    std::unique_ptr<annotation::StopInfoTable> _stop_info_annotation;

    // graph measures
    std::unique_ptr<algorithm::StronglyConnectedComponent> _components;

    // plugins
    std::map<std::string, std::shared_ptr<service::Interface>> plugins;
};

} // namespace service
} // namespace transit

#endif // TRANSIT_SERVICE_MASTER_HPP_
