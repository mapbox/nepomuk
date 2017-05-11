#include "tool/io/binary_decoder.hpp"
#include "tool/io/binary_encoder.hpp"
#include "tool/io/serialisation.hpp"
#include "tool/io/std_stream.hpp"
#include "tool/io/stream_errors.hpp"

#include "tool/io/csv_decoder.hpp"
#include "tool/io/std_line_stream.hpp"

#include <fstream>
#include <ios>
#include <string>
#include <vector>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace nepomuk;

namespace
{
const constexpr char *trip_fixture = "trip.fixture";
void makeTripFixture()
{
    std::ofstream ofs(trip_fixture);
    tool::io::StdLineOutputStream outstream(ofs);
    outstream.putline(
        "\"route_id\",\"service_id\",\"trip_id\",\"trip_headsign\",\"trip_short_name\","
        "\"direction_id\",\"block_id\",\"shape_id\"");
    outstream.putline("17289_700,1,59129500,\"Berlin, S+U "
                      "Alexanderplatz Bhf/Memhardstr. (Berlin)\",\"\",\"0\",,48");
    outstream.putline("17289_700,1,59129500,\"Berlin, S+U "
                      "Alexanderplatz Bhf/Memhardstr. (Berlin)\",\"\",\"\",,48");
    outstream.putline("17289_700,1,59129500,\"Berlin, S+U "
                      "Alexanderplatz Bhf/Memhardstr. (Berlin)\",\"\",,,48");
}

const constexpr char *header_missmatch_fixture = "missmatch.fixture";
void makeHeaderMissmatch()
{
    std::ofstream ofs(header_missmatch_fixture);
    tool::io::StdLineOutputStream outstream(ofs);
    outstream.putline("first,second,third");
    outstream.putline("1,2,3,4");
}

const constexpr char *header_missmatch_under_fixture = "missmatch_under.fixture";
void makeHeaderMissmatchUnder()
{
    std::ofstream ofs(header_missmatch_under_fixture);
    tool::io::StdLineOutputStream outstream(ofs);
    outstream.putline("first,second,third");
    outstream.putline("1,2");
}

} // namespace

BOOST_AUTO_TEST_CASE(write_and_read_bytes)
{
    std::string test{"Don't panic!"};

    { // write data to test file
        std::ofstream ofs("write_and_read_bytes.tmp");
        tool::io::StdOutputStream outstream(ofs);
        auto binary_encoder = tool::io::makeBinaryEncoder(outstream);
        tool::io::serialize(test, binary_encoder);
    }

    std::string read_data = []() {
        std::ifstream ifs("write_and_read_bytes.tmp");
        tool::io::StdInputStream instream(ifs);
        auto binary_decoder = tool::io::makeBinaryDecoder(instream);
        std::string result;
        tool::io::deserialize(result, binary_decoder);
        return result;
    }();

    BOOST_CHECK_EQUAL(test, read_data);
}

BOOST_AUTO_TEST_CASE(handle_unopened_streams)
{
    std::ofstream ofs;
    tool::io::StdOutputStream outstream(ofs);
    auto binary_encoder = tool::io::makeBinaryEncoder(outstream);
    std::string test{"Don't panic!"};

    BOOST_CHECK_THROW(tool::io::serialize(test, binary_encoder),
                      tool::io::OutputStreamError);

    std::ifstream ifs;
    tool::io::StdInputStream instream(ifs);
    auto binary_decoder = tool::io::makeBinaryDecoder(instream);
    std::string result;
    BOOST_CHECK_THROW(tool::io::deserialize(result, binary_decoder),
                      tool::io::InputStreamError);
}

BOOST_AUTO_TEST_CASE(zero_length)
{
    std::string test;

    { // write data to test file
        std::ofstream ofs("write_and_read_bytes.tmp");
        tool::io::StdOutputStream outstream(ofs);
        auto binary_encoder = tool::io::makeBinaryEncoder(outstream);
        tool::io::serialize(test, binary_encoder);
    }

    std::string read_data = []() {
        std::ifstream ifs("write_and_read_bytes.tmp");
        tool::io::StdInputStream instream(ifs);
        auto binary_decoder = tool::io::makeBinaryDecoder(instream);
        std::string result;
        tool::io::deserialize(result, binary_decoder);
        return result;
    }();

    BOOST_CHECK_EQUAL(test, read_data);
}

BOOST_AUTO_TEST_CASE(broken_encode)
{
    std::ofstream ofs("write_and_read_bytes.tmp");
    tool::io::StdOutputStream outstream(ofs);
    BOOST_CHECK_NO_THROW(outstream.write_bytes(NULL, 64));

    std::ifstream ifs("write_and_read_bytes.tmp");
    tool::io::StdInputStream instream(ifs);
    BOOST_CHECK_NO_THROW(instream.read_bytes(NULL, 64));

    BOOST_CHECK(static_cast<bool>(ofs));
    BOOST_CHECK(static_cast<bool>(ifs));
}

BOOST_AUTO_TEST_CASE(read_csv_fixture)
{
    makeTripFixture();

    std::ifstream ifs(trip_fixture);
    tool::io::StdLineInputStream instream(ifs);

    auto decoder = tool::io::makeCSVDecoder(instream, ",");

    std::vector<std::string> tokens;
    std::string expected_header[] = {"route_id",
                                     "service_id",
                                     "trip_id",
                                     "trip_headsign",
                                     "trip_short_name",
                                     "direction_id",
                                     "block_id",
                                     "shape_id"};

    BOOST_CHECK(decoder.header.size() == 8);
    for (std::size_t i = 0; i < 8; ++i)
        BOOST_CHECK(decoder.header.count(expected_header[i]));

    tool::io::deserialize(tokens, decoder);
    BOOST_CHECK(tokens.size() == 8);
    std::string expected_tokens[] = {"17289_700",
                                     "1",
                                     "59129500",
                                     "Berlin, S+U Alexanderplatz Bhf/Memhardstr. (Berlin)",
                                     "",
                                     "0",
                                     "",
                                     "48"};
    for (std::size_t i = 0; i < 8; ++i)
        BOOST_CHECK_EQUAL(expected_tokens[i], tokens[i]);

    // decode three lines, next to the header.
    tool::io::deserialize(tokens, decoder);
    tool::io::deserialize(tokens, decoder);
    tool::io::deserialize(tokens, decoder);
    BOOST_CHECK(tokens.empty());
}

BOOST_AUTO_TEST_CASE(read_csv_stream_not_set)
{
    std::ifstream ifs;
    tool::io::StdLineInputStream instream(ifs);

    auto decoder = tool::io::makeCSVDecoder(instream, ",");

    std::vector<std::string> tokens;
    BOOST_CHECK(decoder.header.empty());
    tool::io::deserialize(tokens, decoder);
    BOOST_CHECK(tokens.empty());
}

BOOST_AUTO_TEST_CASE(failing_stream_check)
{
    std::ifstream ifs;
    ifs.setstate(std::ios::failbit);
    std::string line;
    tool::io::StdLineInputStream instream(ifs);
    BOOST_CHECK_THROW(instream.getline(line), tool::io::InputStreamError);
    BOOST_CHECK(line.empty());
}

BOOST_AUTO_TEST_CASE(dont_write_newlines_in_line_stream)
{
    {
        tool::io::StdLineOutputStream outstream(std::cout);
        BOOST_CHECK_THROW(outstream.putline("Test For Throw\n"),
                          tool::io::OutputStreamError);
        BOOST_CHECK_THROW(outstream.putline("Test For Throw\r"),
                          tool::io::OutputStreamError);
        BOOST_CHECK_NO_THROW(outstream.putline(""));
    }

    // check without stream
    {
        std::ofstream ofs;
        tool::io::StdLineOutputStream outstream(ofs);
        BOOST_CHECK_THROW(outstream.putline("Test For Throw"),
                          tool::io::OutputStreamError);
    }
}

BOOST_AUTO_TEST_CASE(header_missmatch)
{
    makeHeaderMissmatch();
    std::ifstream ifs(header_missmatch_fixture);
    tool::io::StdLineInputStream instream(ifs);

    std::vector<std::string> tokens;
    auto decoder = tool::io::makeCSVDecoder(instream, ",");
    BOOST_CHECK_THROW(tool::io::deserialize(tokens, decoder),
                      tool::io::InvalidFileError);
}

BOOST_AUTO_TEST_CASE(header_missmatch_underflow)
{
    makeHeaderMissmatchUnder();
    std::ifstream ifs(header_missmatch_fixture);
    tool::io::StdLineInputStream instream(ifs);

    std::vector<std::string> tokens;
    auto decoder = tool::io::makeCSVDecoder(instream, ",");
    BOOST_CHECK_THROW(tool::io::deserialize(tokens, decoder),
                      tool::io::InvalidFileError);
}
