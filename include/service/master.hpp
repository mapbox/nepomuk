#ifndef NEPOMUK_SERVICE_MASTER_HPP_
#define NEPOMUK_SERVICE_MASTER_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "gtfs/dataset.hpp"

// the basic timetable
#include "timetable/segment_table.hpp"
#include "timetable/timetable.hpp"

// look-ups
#include "search/coordinate_to_stop.hpp"
#include "search/stop_to_line.hpp"

// pure data
#include "tool/container/string_table.hpp"

// annotation
#include "annotation/api.hpp"
#include "annotation/geometry.hpp"
#include "annotation/pbf.hpp"

#include "annotation/line.hpp"
#include "annotation/stop.hpp"

// graph measures
#include "algorithm/scc.hpp"

namespace nepomuk
{
namespace service
{

// The master service is a meta-service that takes care of all datastructure creation. Many
// different plugins require the same data-structures. The master service can create them and keeps
// them alive.
class Master
{
  public:
    Master(std::string const &path);

    // routing data
    timetable::TimeTable const &timetable();

    // look-ups
    search::StopToLine const &stop_to_line();
    search::CoordinateToStop const &coordinate_to_stop();

    // plain data
    tool::container::StringTable const &dictionary();
    timetable::SegmentTable const &segment_table();

    // annotation
    annotation::Geometry const &geometry_annotation();
    annotation::API const &api_annotation();
    annotation::PBF const &pbf_annotation();

    annotation::Stop const &stop_annotation();
    annotation::Line const &line_annotation();

    // graph measurements
    algorithm::StronglyConnectedComponent const &components();

  private:
    std::vector<std::size_t> const &trip_offsets_by_line();

    gtfs::Dataset base_data;
    std::unique_ptr<std::vector<std::size_t>> _trip_offsets_by_line;

    // timetable
    std::unique_ptr<timetable::TimeTable> _timetable;

    // look-ups
    std::unique_ptr<search::StopToLine> _stop_to_line;
    std::unique_ptr<search::CoordinateToStop> _coordinate_to_stop;

    // plain data
    std::unique_ptr<tool::container::StringTable> _dictionary;
    std::unique_ptr<timetable::SegmentTable> _segment_table;

    // annotation
    std::unique_ptr<annotation::Geometry> _geometry_annotation;
    std::unique_ptr<annotation::Stop> _stop_annotation;
    std::unique_ptr<annotation::Line> _line_annotation;

    std::unique_ptr<annotation::API> _api_annotation;
    std::unique_ptr<annotation::PBF> _pbf_annotation;

    // graph measures
    std::unique_ptr<algorithm::StronglyConnectedComponent> _components;
};

} // namespace service
} // namespace nepomuk

#endif // NEPOMUK_SERVICE_MASTER_HPP_
