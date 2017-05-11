#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "id/agency.hpp"
#include "id/stop.hpp"
#include "id/trip.hpp"
#include "tool/container/id_hasher.hpp"

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace nepomuk;

const constexpr int num_ids = 10;

BOOST_AUTO_TEST_CASE(handle_different_ids)
{
    std::vector<int> data(num_ids);
    std::iota(data.begin(), data.end(), num_ids);
    std::random_shuffle(data.begin(), data.end());

    for (std::uint32_t i = 0; i < num_ids; ++i)
        BOOST_CHECK_EQUAL(
            tool::container::id_hash::get_id<AgencyID>(std::to_string(data[i]), "default"),
            AgencyID{i});

    // make sure IDS don't change between hashes
    for (std::uint32_t i = 0; i < num_ids; ++i)
        BOOST_CHECK_EQUAL(
            tool::container::id_hash::get_id<AgencyID>(std::to_string(data[i]), "default"),
            AgencyID{i});

    // reshuffle, check if types actually get their own IDs
    std::random_shuffle(data.begin(), data.end());
    for (std::uint64_t i = 0; i < num_ids; ++i)
        BOOST_CHECK_EQUAL(
            tool::container::id_hash::get_id<StopID>(std::to_string(data[i]), "default"),
            StopID{i});
}

BOOST_AUTO_TEST_CASE(handle_different_sources)
{
    std::vector<int> data(num_ids);
    std::iota(data.begin(), data.end(), num_ids);
    std::random_shuffle(data.begin(), data.end());

    for (std::uint32_t i = 0; i < num_ids; ++i)
        BOOST_CHECK_EQUAL(
            tool::container::id_hash::get_id<TripID>(std::to_string(data[i]), "default"),
            TripID{i});

    std::random_shuffle(data.begin(), data.end());
    // make sure IDS don't change between hashes
    for (std::uint32_t i = 0; i < num_ids; ++i)
        BOOST_CHECK_EQUAL(tool::container::id_hash::get_id<TripID>(std::to_string(data[i]),
                                                                            "other_source"),
                          TripID{i + num_ids});
}
