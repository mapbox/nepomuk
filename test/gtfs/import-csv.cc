#include "gtfs/agency.hpp"
#include "gtfs/fare.hpp"
#include "gtfs/read_csv.hpp"
#include "gtfs/route.hpp"
#include "gtfs/sequence.hpp"
#include "gtfs/service.hpp"
#include "gtfs/shape.hpp"
#include "gtfs/stop.hpp"
#include "gtfs/trip.hpp"
#include "gtfs/zone.hpp"

#include "tool/container/dictionary.hpp"

#include <iostream>
#include <string>

using namespace transit::gtfs;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

void prepareFixture(std::string const &name)
{
    boost::filesystem::path dir(name);
    boost::filesystem::create_directory(dir);
}

void makeInvalidAgency(std::string const &name)
{
    std::ofstream ofs(name + "/agency.txt");
    // presenting 'agencyname' instead of 'agencyname'. Should throw.
    ofs << "\"agencyname\",\"agency_url\",\"agency_timezone\"D,\"agency_lang\","
           "\"agency_phone\"\n";
    ofs << "\"S-Bahn Berlin "
           "GmbH\",\"http://www.s-bahn-berlin.de\",\"Europe/Berlin\",\"de\",\"\"\n";
}

void makeAgency(std::string const &name)
{
    std::ofstream ofs(name + "/agency.txt");
    ofs << "\"agency_id\",\"agency_name\",\"agency_url\",\"agency_timezone\",\"agency_lang\","
           "\"agency_phone\",\"agency_fare_url\",\"agency_email\r\"\n";
    ofs << "1,\"S-Bahn Berlin "
           "GmbH\",\"http://www.s-bahn-berlin.de\",\"Europe/"
           "Berlin\",\"de\",\"030\",\"www.fare.url\",\"mail@agency.de\r\"\n";
}

void checkAgencyFixture(Agency const &agency)
{
    const auto valid_id = agency.id && *agency.id == AgencyID{0};
    BOOST_CHECK(valid_id);
    const auto valid_name = agency.name == "S-Bahn Berlin GmbH";
    BOOST_CHECK(valid_name);
    const auto valid_url = agency.url == "http://www.s-bahn-berlin.de";
    BOOST_CHECK(valid_url);
    const auto valid_timezone = agency.timezone == "Europe/Berlin";
    BOOST_CHECK(valid_timezone);
    const auto valid_language = agency.language && *agency.language == "de";
    BOOST_CHECK(valid_language);
    const auto valid_phone = agency.phone && *agency.phone == "030";
    BOOST_CHECK(valid_phone);
    const auto valid_fare_url = agency.fare_url && *agency.fare_url == "www.fare.url";
    BOOST_CHECK(valid_fare_url);
    const auto valid_mail = agency.email && *agency.email == "mail@agency.de";
    BOOST_CHECK(valid_mail);
}

void makeMinimalAgency(std::string const &name)
{
    std::ofstream ofs(name + "/agency.txt");
    ofs << "\"agency_name\",\"agency_url\",\"agency_timezone\"\n";
    ofs << "\"S-Bahn Berlin GmbH\",\"http://www.s-bahn-berlin.de\",\"Europe/Berlin\r\"\n";
}

void makeMinimalAgencyInStops(std::string const &name)
{
    std::ofstream ofs(name + "/stops.txt");
    ofs << "\"agency_name\",\"agency_url\",\"agency_timezone\"\n";
    ofs << "\"S-Bahn Berlin GmbH\",\"http://www.s-bahn-berlin.de\",\"Europe/Berlin\"\n";
}

void checkMinimalAgencyFixture(Agency const &agency)
{
    const auto valid_name = agency.name == "S-Bahn Berlin GmbH";
    BOOST_CHECK(valid_name);
    const auto valid_url = agency.url == "http://www.s-bahn-berlin.de";
    BOOST_CHECK(valid_url);
    const auto valid_timezone = agency.timezone == "Europe/Berlin";
    BOOST_CHECK(valid_timezone);

    const auto no_optionals = !agency.id && !agency.language && !agency.phone && !agency.email;
    BOOST_CHECK(no_optionals);
}

void makeStops(std::string const &name)
{
    std::ofstream ofs(name + "/stops.txt");
    ofs << "\"stop_id\",\"stop_code\",\"stop_name\",\"stop_desc\",\"stop_lat\",\"stop_lon\","
           "\"location_type\",\"parent_station\",\"zone_id\",\"stop_url\",\"stop_timezone\","
           "\"wheelchair_boarding\"\n";
    ofs << "000008012716,\"123\",\"Rastow, "
           "Bahnhof\",\"its\",\"1.0000000\",\"1.0000000\",0,00000550215,1,,\"de\",0\n";
    ofs << "000008012717,\"456\",\"Restow, "
           "Bahnhof\",\"so beautiful\",\"2.0000000\",\"2.0000000\",1,00000550216,2,\"www.someurl."
           "com\",\"de\",1\n";
    ofs << "000008012718,\"\",\"Ristow, "
           "Bahnhof\",,\"3.0000000\",\"3.0000000\",,00000550217,3,,\"us\",2\n";
}

void checkStopFixture(std::vector<Stop> const &stops,
                      transit::tool::container::Dictionary const &dictionary)
{
    // we expect exactly three stops, see above
    BOOST_CHECK_EQUAL(stops.size(), 3);

    // stops include parent stop ids in every location
    const auto valid_ids =
        (stops[0].id == StopID{0}) && (stops[1].id == StopID{2}) && (stops[2].id == StopID{4});

    BOOST_CHECK(valid_ids);

    const auto valid_codes = stops[0].code && (*stops[0].code == "123") && stops[1].code &&
                             (*stops[1].code == "456") && !stops[2].code;
    BOOST_CHECK(valid_codes);

    const auto valid_names = (dictionary.get_string(stops[0].name) == "Rastow, Bahnhof") &&
                             (dictionary.get_string(stops[1].name) == "Restow, Bahnhof") &&
                             (dictionary.get_string(stops[2].name) == "Ristow, Bahnhof");
    BOOST_CHECK(valid_names);

    auto check_lat_lon = [](auto const lat_or_lon, const std::int32_t expected) {
        return static_cast<std::int32_t>(lat_or_lon) == expected;
    };

    const auto valid_location = check_lat_lon(stops[0].location.longitude, 1000000) &&
                                check_lat_lon(stops[0].location.latitude, 1000000) &&
                                check_lat_lon(stops[1].location.longitude, 2000000) &&
                                check_lat_lon(stops[1].location.latitude, 2000000) &&
                                check_lat_lon(stops[2].location.longitude, 3000000) &&
                                check_lat_lon(stops[2].location.latitude, 3000000);
    BOOST_CHECK(valid_location);

    const auto valid_types =
        stops[0].location_type && (*stops[0].location_type == LocationType::STOP) &&
        stops[1].location_type && (*stops[1].location_type == LocationType::STATION) &&
        stops[2].location_type && (*stops[2].location_type == LocationType::STOP);
    BOOST_CHECK(valid_types);

    const auto valid_parents = stops[0].parent_station && (*stops[0].parent_station == StopID{1}) &&
                               stops[1].parent_station && (*stops[1].parent_station == StopID{3}) &&
                               stops[2].parent_station && (*stops[2].parent_station == StopID{5});
    BOOST_CHECK(valid_parents);

    const auto valid_descriptions =
        stops[0].description && (dictionary.get_string(*stops[0].description) == "its") &&
        stops[1].description && (dictionary.get_string(*stops[1].description) == "so beautiful") &&
        !stops[2].description;
    BOOST_CHECK(valid_descriptions);

    const auto valid_zones = stops[0].zone_id && (stops[0].zone_id == ZoneID{0}) &&
                             stops[1].zone_id && (stops[1].zone_id == ZoneID{1}) &&
                             stops[2].zone_id && (stops[2].zone_id == ZoneID{2});
    BOOST_CHECK(valid_zones);

    const auto valid_urls = !stops[0].url && stops[1].url &&
                            (dictionary.get_string(*stops[1].url) == "www.someurl.com") &&
                            !stops[2].url;
    BOOST_CHECK(valid_urls);

    const auto valid_timezones = stops[0].timezone && (*stops[0].timezone == "de") &&
                                 stops[1].timezone && (*stops[1].timezone == "de") &&
                                 stops[2].timezone && (*stops[2].timezone == "us");
    BOOST_CHECK(valid_timezones);

    const auto valid_wheelchair =
        stops[0].wheelchair_access &&
        (*stops[0].wheelchair_access == accessibility::Wheelchair::INHERIT_OR_NONE) &&
        stops[1].wheelchair_access &&
        (*stops[1].wheelchair_access == accessibility::Wheelchair::SOME_OR_ALL) &&
        stops[2].wheelchair_access &&
        (*stops[2].wheelchair_access == accessibility::Wheelchair::NONE);
    BOOST_CHECK(valid_wheelchair);
}

void makeMinimalStops(std::string const &name)
{
    std::ofstream ofs(name + "/stops.txt");
    ofs << "\"stop_id\",\"stop_name\",\"stop_lat\",\"stop_lon\"\n";
    ofs << "000008012716,\"Rastow, "
           "Bahnhof\",\"1.0\",\"1.000000\"\n";
    ofs << "000008012717,\"Restow, "
           "Bahnhof\",\"2.00000\",\"2.00000\"\n";
    ofs << "000008012718,\"Ristow, "
           "Bahnhof\",\"3.00000\",\"3.00000\"\n";
}
void checkMinimalStopFixture(std::vector<Stop> const &stops,
                             transit::tool::container::Dictionary const &dictionary)
{
    // we expect exactly three stops, see above
    BOOST_CHECK_EQUAL(stops.size(), 3);

    // minimal IDs do not conatin parent stop stations
    const auto valid_ids =
        (stops[0].id == StopID{0}) && (stops[1].id == StopID{2}) && (stops[2].id == StopID{4});
    BOOST_CHECK(valid_ids);

    const auto valid_names = (dictionary.get_string(stops[0].name) == "Rastow, Bahnhof") &&
                             (dictionary.get_string(stops[1].name) == "Restow, Bahnhof") &&
                             (dictionary.get_string(stops[2].name) == "Ristow, Bahnhof");
    BOOST_CHECK(valid_names);

    auto check_lat_lon = [](auto const lat_or_lon, const std::int32_t expected) {
        return static_cast<std::int32_t>(lat_or_lon) == expected;
    };

    const auto valid_location = check_lat_lon(stops[0].location.longitude, 1000000) &&
                                check_lat_lon(stops[0].location.latitude, 1000000) &&
                                check_lat_lon(stops[1].location.longitude, 2000000) &&
                                check_lat_lon(stops[1].location.latitude, 2000000) &&
                                check_lat_lon(stops[2].location.longitude, 3000000) &&
                                check_lat_lon(stops[2].location.latitude, 3000000);
    BOOST_CHECK(valid_location);

    const auto no_optionals = [](Stop const &stop) {
        return !stop.code && !stop.description && !stop.zone_id && !stop.url &&
               !stop.location_type && !stop.parent_station && !stop.timezone &&
               !stop.wheelchair_access;
    };

    BOOST_CHECK(no_optionals(stops[0]) && no_optionals(stops[1]) && no_optionals(stops[2]));
}

void makeMinimalStopsInAgency(std::string const &name)
{
    std::ofstream ofs(name + "/agency.txt");
    ofs << "\"stop_id\",\"stop_name\",\"stop_lat\",\"stop_lon\"\n";
    ofs << "000008012716,\"Rastow, "
           "Bahnhof\",\"53.457379000000\",\"11.431163000000\"\n";
    ofs << "000008012716,\"Rastow, "
           "Bahnhof\",\"53.457379000000\",\"11.431163000000\"\n";
    ofs << "000008012716,\"Rastow, "
           "Bahnhof\",\"53.457379000000\",\"11.431163000000\"\n";
}

void makeRoutes(std::string const &name)
{
    std::ofstream ofs(name + "/routes.txt");
    ofs << "\"route_id\",\"agency_id\",\"route_short_name\",\"route_long_name\",\"route_type\","
           "\"route_url\",\"route_color\",\"route_text_color\",\"route_desc\"\n";
    ofs << "17289_700,796,\"100\",\"10\",700,,,,\"most beautiful route\"\n";
    ofs << "17289_700,796,\"101\",\"11\",700,\"www.route.url\",\"#001122\",\"#221100\",\"great "
           "route\"\n";
}

void makeMinimalRoutes(std::string const &name)
{
    std::ofstream ofs(name + "/routes.txt");
    ofs << "\"route_id\",\"route_short_name\",\"route_long_name\",\"route_type\"\n";
    ofs << "17289_700,\"100\",\"100\",700\n";
}

void makeTrips(std::string const &name)
{
    std::ofstream ofs(name + "/trips.txt");
    ofs << "\"route_id\",\"service_id\",\"trip_id\",\"trip_headsign\",\"trip_short_name\","
           "\"direction_id\",\"block_id\",\"shape_id\",\"bikes_allowed\",\"wheelchair_"
           "accessible\r\"\n";
    ofs << "17289_700,1,59129500,\"Berlin, S+U Alexanderplatz Bhf/Memhardstr. "
           "(Berlin)\",\"\",\"0\",,48,,0\n";
    ofs << "17289_700,1,59129500,\"Berlin, S+U Alexanderplatz Bhf/Memhardstr. "
           "(Berlin)\",\"\",,,48,0,1\n";
    ofs << "17289_700,1,59129500,\"Berlin, S+U Alexanderplatz Bhf/Memhardstr. "
           "(Berlin)\",\"\",,,48,0,1\n";
    ofs << "17289_700,1,59129500,\"Berlin, S+U Alexanderplatz Bhf/Memhardstr. "
           "(Berlin)\",\"\",\"1\",,48,1,2\n";
    ofs << "17289_700,1,59129500,\"Berlin, S+U Alexanderplatz Bhf/Memhardstr. "
           "(Berlin)\",\"\",\"1\",,48,2,\n";
}
void makeMinimalTrips(std::string const &name)
{
    std::ofstream ofs(name + "/trips.txt");
    ofs << "\"route_id\",\"service_id\",\"trip_id\"\n";
    ofs << "17289_700,1,59129500";
}

void makeStopTimes(std::string const &name)
{
    std::ofstream ofs(name + "/stop_times.txt");
    ofs << "\"trip_id\",\"arrival_time\",\"departure_time\",\"stop_id\",\"stop_sequence\",\"pickup_"
           "type\",\"drop_off_type\",\"stop_headsign\",\"timepoint\",\"shape_dist_traveled\"\n";
    ofs << "59129500,6:29:00,6:29:00,070101003212,0,0,0,\"\",,0.1103\n";
    ofs << "59129500,6:29:00,6:29:00,070101003212,0,,1,\"\",0,0.1103\n";
    ofs << "59129500,6:29:00,6:29:00,070101003212,0,2,,\"\",1,0.1103\n";
    ofs << "61699126,6:29:00,6:29:00,070101003212,0,1,2,\"S+U Alexanderplatz\",,0.1103\n";
    ofs << "61699126,6:29:00,6:29:00,070101003212,0,3,3,\"S+U Alexanderplatz\",,0.1103\n";
}
void makeMinimalStopTimes(std::string const &name)
{
    std::ofstream ofs(name + "/stop_times.txt");
    ofs << "\"trip_id\",\"arrival_time\",\"departure_time\",\"stop_id\",\"stop_sequence\"\n";
    ofs << "59129500,6:29:00,6:29:00,070101003212,0\n";
    ofs << "59129500,6:29:00,6:29:00,070101003212,1\n";
    ofs << "59129500,6:29:00,6:29:00,070101003212,2\r\n";
    ofs << "61699126,6:29:00,6:29:00,070101003212,3\n";
    ofs << "61699126,6:29:00,6:29:00,070101003212,4\n";
}

void makeCalendar(std::string const &name)
{
    std::ofstream ofs(name + "/calendar.txt");
    ofs << "\"service_id\",\"monday\",\"tuesday\",\"wednesday\",\"thursday\",\"friday\","
           "\"saturday\",\"sunday\",\"start_date\",\"end_date\"\n";
    ofs << "1,1,1,1,1,1,0,0,20170119,20171209\n";
    ofs << "1,0,0,0,0,0,1,1,20170119,20171209\n";
}

void makeRequired(std::string const &name)
{
    makeAgency(name);
    makeStops(name);
    makeRoutes(name);
    makeTrips(name);
    makeStopTimes(name);
    makeCalendar(name);
}

void makeMinimalRequired(std::string const &name)
{
    makeMinimalAgency(name);
    makeMinimalStops(name);
    makeMinimalRoutes(name);
    makeMinimalTrips(name);
    makeMinimalStopTimes(name);
    makeCalendar(name);
}

void makeMinimalRequiredSwitched(std::string const &name)
{
    makeMinimalAgencyInStops(name);
    makeMinimalStopsInAgency(name);
    makeMinimalRoutes(name);
    makeMinimalTrips(name);
    makeMinimalStopTimes(name);
    makeCalendar(name);
}

void makeInvalidRequired(std::string const &name)
{
    makeInvalidAgency(name);
    makeStops(name);
    makeRoutes(name);
    makeTrips(name);
    makeStopTimes(name);
    makeCalendar(name);
}

void checkDatasetRequired(Dataset const &dataset)
{
    BOOST_CHECK(dataset.agencies.size() == 1);
    checkAgencyFixture(dataset.agencies.front());
    checkStopFixture(dataset.stops, dataset.dictionary);
}

void checkMinimalDataset(Dataset const &dataset)
{
    BOOST_CHECK(dataset.agencies.size() == 1);
    checkMinimalAgencyFixture(dataset.agencies.front());
    checkMinimalStopFixture(dataset.stops, dataset.dictionary);
}
void makeCalendarDates(std::string const &name)
{
    std::ofstream ofs(name + "/calendar_dates.txt");
    ofs << "\"service_id\",\"date\",\"exception_type\"\n";
    ofs << "1,20170123,2\n";
    ofs << "1,20170123,1\n";
}

void makeFareRules(std::string const &name)
{
    std::ofstream ofs(name + "/fare_rules.txt");
    ofs << "\"fare_id\",\"route_id\",\"origin_id\",\"destination_id\",\"contains_id\"\n";
    ofs << "1,2,1,3,\n";
}

void makeFareAttributes(std::string const &name)
{
    std::ofstream ofs(name + "/fare_attributes.txt");
    ofs << "\"fare_id\",\"price\",\"currency_type\",\"payment_method\",\"transfers\",\"transfer_"
           "duration\"\n";
    ofs << "1,2.3,EUR,0,,23\n";
    ofs << "1,2.3,EUR,0,0,23\n";
    ofs << "1,2.3,EUR,1,1,23\n";
    ofs << "1,2.3,EUR,1,2,23\n";
}

void makeShapes(std::string const &name)
{
    std::ofstream ofs(name + "/shapes.txt");
    ofs << "\"shape_id\",\"shape_pt_lat\",\"shape_pt_lon\",\"shape_pt_sequence\"\n";
    ofs << "1406,52.390931,13.067157,0\n";
    ofs << "1406,52.390931,13.067157,1\n";
}

void makeFrequencies(std::string const &name)
{
    std::ofstream ofs(name + "/frequencies.txt");
    ofs << "\"trip_id\",\"start_time\",\"end_time\",\"headway_secs\",\"exact_times\"\n";
    ofs << "1,13:10:57,25:08:11,200,1\n";
    ofs << "1,13:10:57,25:08:11,200,0\n";
    ofs << "1,13:10:57,25:08:11,200,\n";
}

void makeTransfers(std::string const &name)
{
    std::ofstream ofs(name + "/transfers.txt");
    ofs << "\"from_stop_id\",\"to_stop_id\",\"transfer_type\",\"min_transfer_time\",\"from_route_"
           "id\",\"to_route_id\",\"from_trip_id\",\"to_trip_id\"\n";
    ofs << "000008012716,000008012716,,1300,,,,\n";
    ofs << "000008012716,000008012716,1,2300,,,,\n";
    ofs << "000008012716,000008012716,2,4300,,,,\n";
    ofs << "000008012716,000008012716,3,5300,,,,\n";
    ofs << "000008010327,000008010327,2,240,,,61814935,61815841\n";
}

void makeFeedInfo(std::string const &name)
{
    std::ofstream ofs(name + "/feed_info.txt");
    ofs << "\"feed_publisher_name\",\"feed_publisher_url\",\"feed_lang\",\"feed_start_date\","
           "\"feed_end_date\",\"feed_version\"\n";
    ofs << "\"Some Nice "
           "Publisher\",\"www.publisher.url/not_found/\",\"GER\",20170101,20171212,\"v0.2.1\"\n";
}

void makeOptional(std::string const &name)
{
    makeCalendarDates(name);
    makeFareAttributes(name);
    makeFareRules(name);
    makeShapes(name);
    makeFrequencies(name);
    makeTransfers(name);
    makeFeedInfo(name);
}

void cleanUp(std::string const &name)
{
    boost::filesystem::path dir(name);
    remove_all(dir);
}

// create a datasource to read in
void makeFixture(std::string const &name)
{
    prepareFixture(name);
    makeRequired(name);
    makeOptional(name);
}

BOOST_AUTO_TEST_CASE(check_missing_required)
{
    std::string const name = "missing_required_files";
    prepareFixture(name);
    makeOptional(name);
    BOOST_CHECK_THROW(CSVDiscSource source(name), transit::tool::io::FileNotFoundError);
    cleanUp(name);
}

BOOST_AUTO_TEST_CASE(read_csv)
{
    std::string const name = "valid";
    makeFixture(name);
    CSVDiscSource source(name);
    auto control = readCSV(source);
    checkDatasetRequired(control);
    cleanUp(name);
}

BOOST_AUTO_TEST_CASE(invalid_header_throws)
{
    std::string const name = "invalid_header";
    prepareFixture(name);
    makeInvalidRequired(name);
    CSVDiscSource source(name);
    BOOST_CHECK_THROW(readCSV(source), transit::tool::io::InvalidFileError);
    cleanUp(name);
}

BOOST_AUTO_TEST_CASE(read_minimal)
{
    std::string const name = "minimal";
    prepareFixture(name);
    makeMinimalRequired(name);
    CSVDiscSource source(name);
    auto control = readCSV(source);
    checkMinimalDataset(control);
    cleanUp(name);
}

BOOST_AUTO_TEST_CASE(read_minimal_one_off)
{
    std::string const name = "minimal_one_off";
    prepareFixture(name);
    makeMinimalRequired(name);
    makeMinimalStopsInAgency(name);
    CSVDiscSource source(name);
    BOOST_CHECK_THROW(readCSV(source), transit::tool::io::InvalidFileError);
    cleanUp(name);
}

BOOST_AUTO_TEST_CASE(check_switched_header)
{
    std::string const name = "parsing_wrong_file";
    prepareFixture(name);
    makeMinimalRequiredSwitched(name);

    CSVDiscSource source(name);
    BOOST_CHECK_THROW(readCSV(source), transit::tool::io::InvalidFileError);

    cleanUp(name);
}
