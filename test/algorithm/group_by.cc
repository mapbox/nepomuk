#include "algorithm/group_by.hpp"

#include <utility>
#include <vector>

#include <iostream>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(handle_unopened_streams)
{
    std::vector<int> data = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

    using itr = std::vector<int>::iterator;
    using range = std::pair<itr, itr>;

    std::vector<range> result;

    const auto add_to_results = [&result](const range rng) { result.push_back(rng); };

    transit::algorithm::grouped_by(
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
