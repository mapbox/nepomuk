#include <iterator>
#include <string>

#include "tool/container/surjective_mapping.hpp"

using namespace transit::tool::container;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(mapping_functionality)
{
    SurjectiveMapping<int, int> mapping(4, 2);
    mapping.map_codomain(0, 0);
    mapping.map_codomain(1, 0);
    mapping.map_codomain(2, 0);
    mapping.map_codomain(3, 1);

    BOOST_CHECK_EQUAL(mapping.codomain_mapping(0), 0);
    BOOST_CHECK_EQUAL(mapping.codomain_mapping(1), 0);
    BOOST_CHECK_EQUAL(mapping.codomain_mapping(2), 0);
    BOOST_CHECK_EQUAL(mapping.codomain_mapping(3), 1);

    auto itr_range = mapping.domain_mapping(0);
    BOOST_CHECK_EQUAL(std::distance(itr_range.begin(), itr_range.end()), 3);
    auto itr_range_2 = mapping.domain_mapping(1);
    BOOST_CHECK_EQUAL(std::distance(itr_range_2.begin(), itr_range_2.end()), 1);
    BOOST_CHECK_EQUAL(mapping.domain_size(), 2);
    BOOST_CHECK_EQUAL(mapping.codomain_size(), 4);
}
