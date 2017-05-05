#include "algorithm/ranges.hpp"

#include <utility>
#include <vector>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using itr = std::vector<int>::iterator;
using range = std::pair<itr, itr>;

bool even(int val) { return val % 2 == 0; }

BOOST_AUTO_TEST_CASE(by_equal_ranges)
{
    std::vector<int> data = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
    std::vector<range> result;
    const auto add_to_results = [&result](const range rng) { result.push_back(rng); };

    transit::algorithm::by_equal_ranges(
        data.begin(),
        data.end(),
        [](const int value, const int candidate) { return value < candidate; },
        add_to_results);

    BOOST_CHECK_EQUAL(result.size(), 4ull);
    BOOST_CHECK_EQUAL(std::distance(result[0].first, result[0].second), 1);
    BOOST_CHECK_EQUAL(std::distance(result[1].first, result[1].second), 2);
    BOOST_CHECK_EQUAL(std::distance(result[2].first, result[2].second), 3);
    BOOST_CHECK_EQUAL(std::distance(result[3].first, result[3].second), 4);
}

BOOST_AUTO_TEST_CASE(on_alternating_ranges)
{
    std::vector<int> data = {0, 0, 0, 1, 1, 3, 2, 2, 2, 0, 1, 1};
    std::vector<range> result;
    const auto add_to_results = [&result](const range rng) { result.push_back(rng); };

    transit::algorithm::on_alternating_ranges(data.begin(), data.end(), even, add_to_results);
    BOOST_CHECK_EQUAL(result.size(), 4ull);
    BOOST_CHECK_EQUAL(std::distance(result[0].first, result[0].second), 3);
    BOOST_CHECK_EQUAL(std::distance(result[1].first, result[1].second), 3);
    BOOST_CHECK_EQUAL(std::distance(result[2].first, result[2].second), 4);
    BOOST_CHECK_EQUAL(std::distance(result[3].first, result[3].second), 2);

    transit::algorithm::on_alternating_ranges(data.begin(), data.begin(), even, add_to_results);
    BOOST_CHECK_EQUAL(result.size(), 4ull);
}

BOOST_AUTO_TEST_CASE(on_ranges)
{
    std::vector<int> data = {0, 0, 0, 1, 1, 3, 2, 2, 2, 0, 1, 1};
    std::vector<range> result;
    const auto add_to_results = [&result](const range rng) { result.push_back(rng); };

    transit::algorithm::on_ranges(data.begin(), data.end(), even, add_to_results);
    BOOST_CHECK_EQUAL(result.size(), 2ull);
    BOOST_CHECK_EQUAL(std::distance(result[0].first, result[0].second), 3);
    BOOST_CHECK_EQUAL(std::distance(result[1].first, result[1].second), 4);
}
