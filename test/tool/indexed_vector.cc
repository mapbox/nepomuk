#include <string>

#include "tool/container/indexed_vector.hpp"

using namespace transit::tool::container;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

auto construct()
{
    IndexedVector<int> vec;
    vec.reserve(10);

    vec.push_back(0, 1);
    vec.push_back(0, 2);
    vec.push_back(0, 3);
    vec.push_back(1, 3);
    vec.push_back(1, 2);
    vec.push_back(1, 1);
    vec.push_back(3, 0);
    vec.push_back(2, 4);
    vec.push_back(7, 7);

    return vec;
}

BOOST_AUTO_TEST_CASE(sizes)
{
    auto test_object = construct();
    BOOST_CHECK_EQUAL(test_object.category_size(), 8);
    BOOST_CHECK_EQUAL(test_object.category_size(0), 3);
    BOOST_CHECK_EQUAL(test_object.category_size(1), 3);
    BOOST_CHECK_EQUAL(test_object.category_size(2), 1);
    BOOST_CHECK_EQUAL(test_object.category_size(3), 1);
    BOOST_CHECK_EQUAL(test_object.category_size(4), 0);
    BOOST_CHECK_EQUAL(test_object.category_size(5), 0);
    BOOST_CHECK_EQUAL(test_object.category_size(6), 0);
    BOOST_CHECK_EQUAL(test_object.category_size(7), 1);
}

BOOST_AUTO_TEST_CASE(bracket_access)
{
    auto test_object = construct();
    BOOST_CHECK_EQUAL(test_object[0], 1);
    BOOST_CHECK_EQUAL(test_object[1], 2);
    BOOST_CHECK_EQUAL(test_object[2], 3);
    BOOST_CHECK_EQUAL(test_object[3], 3);
    BOOST_CHECK_EQUAL(test_object[4], 2);
    BOOST_CHECK_EQUAL(test_object[5], 1);
    BOOST_CHECK_EQUAL(test_object[6], 4);
    BOOST_CHECK_EQUAL(test_object[7], 0);
    BOOST_CHECK_EQUAL(test_object[8], 7);

    BOOST_CHECK_EQUAL((test_object[{0, 0}]), 1);
    BOOST_CHECK_EQUAL((test_object[{0, 1}]), 2);
    BOOST_CHECK_EQUAL((test_object[{0, 2}]), 3);

    BOOST_CHECK_EQUAL((test_object[{1, 0}]), 3);
    BOOST_CHECK_EQUAL((test_object[{1, 1}]), 2);
    BOOST_CHECK_EQUAL((test_object[{1, 2}]), 1);

    BOOST_CHECK_EQUAL((test_object[{2, 0}]), 4);
    BOOST_CHECK_EQUAL((test_object[{3, 0}]), 0);
    BOOST_CHECK_EQUAL((test_object[{7, 0}]), 7);
}

BOOST_AUTO_TEST_CASE(const_bracket_access)
{
    auto const test_object = construct();
    BOOST_CHECK_EQUAL(test_object[0], 1);
    BOOST_CHECK_EQUAL(test_object[1], 2);
    BOOST_CHECK_EQUAL(test_object[2], 3);
    BOOST_CHECK_EQUAL(test_object[3], 3);
    BOOST_CHECK_EQUAL(test_object[4], 2);
    BOOST_CHECK_EQUAL(test_object[5], 1);
    BOOST_CHECK_EQUAL(test_object[6], 4);
    BOOST_CHECK_EQUAL(test_object[7], 0);
    BOOST_CHECK_EQUAL(test_object[8], 7);

    BOOST_CHECK_EQUAL((test_object[{0, 0}]), 1);
    BOOST_CHECK_EQUAL((test_object[{0, 1}]), 2);
    BOOST_CHECK_EQUAL((test_object[{0, 2}]), 3);

    BOOST_CHECK_EQUAL((test_object[{1, 0}]), 3);
    BOOST_CHECK_EQUAL((test_object[{1, 1}]), 2);
    BOOST_CHECK_EQUAL((test_object[{1, 2}]), 1);

    BOOST_CHECK_EQUAL((test_object[{2, 0}]), 4);
    BOOST_CHECK_EQUAL((test_object[{3, 0}]), 0);
    BOOST_CHECK_EQUAL((test_object[{7, 0}]), 7);
}

BOOST_AUTO_TEST_CASE(range_access)
{
    auto test_object = construct();

    int expected[] = {1, 2, 3, 3, 2, 1, 4, 0, 7};
    BOOST_CHECK_EQUAL_COLLECTIONS(test_object.begin(), test_object.end(), expected, expected + 9);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        test_object.range().begin(), test_object.range().end(), expected, expected + 9);

    int zero_cat[] = {1, 2, 3};
    int one_cat[] = {3, 2, 1};

    BOOST_CHECK_EQUAL_COLLECTIONS(test_object.begin(0), test_object.end(0), zero_cat, zero_cat + 3);
    BOOST_CHECK_EQUAL_COLLECTIONS(test_object.begin(1), test_object.end(1), one_cat, one_cat + 3);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        test_object.range(0).begin(), test_object.range(0).end(), zero_cat, zero_cat + 3);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        test_object.range(1).begin(), test_object.range(1).end(), one_cat, one_cat + 3);

    int zero_cat_back[] = {2, 3};
    int zero_cat_front[] = {1, 2};
    auto const back_range = test_object.range(0, 1, test_object.category_size(0));
    BOOST_CHECK_EQUAL_COLLECTIONS(
        back_range.begin(), back_range.end(), zero_cat_back, zero_cat_back + 2);
    auto const front_range = test_object.range(0, 0, 2);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        front_range.begin(), front_range.end(), zero_cat_front, zero_cat_front + 2);
}

BOOST_AUTO_TEST_CASE(const_range_access)
{
    auto test_object = construct();

    int expected[] = {1, 2, 3, 3, 2, 1, 4, 0, 7};
    BOOST_CHECK_EQUAL_COLLECTIONS(test_object.cbegin(), test_object.cend(), expected, expected + 9);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        test_object.crange().begin(), test_object.crange().end(), expected, expected + 9);

    int zero_cat[] = {1, 2, 3};
    int one_cat[] = {3, 2, 1};

    BOOST_CHECK_EQUAL_COLLECTIONS(
        test_object.cbegin(0), test_object.cend(0), zero_cat, zero_cat + 3);
    BOOST_CHECK_EQUAL_COLLECTIONS(test_object.cbegin(1), test_object.cend(1), one_cat, one_cat + 3);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        test_object.crange(0).begin(), test_object.crange(0).end(), zero_cat, zero_cat + 3);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        test_object.crange(1).begin(), test_object.crange(1).end(), one_cat, one_cat + 3);

    int zero_cat_back[] = {2, 3};
    int zero_cat_front[] = {1, 2};
    auto const back_range = test_object.crange(0, 1, test_object.category_size(0));
    BOOST_CHECK_EQUAL_COLLECTIONS(
        back_range.begin(), back_range.end(), zero_cat_back, zero_cat_back + 2);
    auto const front_range = test_object.crange(0, 0, 2);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        front_range.begin(), front_range.end(), zero_cat_front, zero_cat_front + 2);
}
