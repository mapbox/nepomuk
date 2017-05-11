#include "algorithm/scc.hpp"
#include "tool/container/forward_star_graph_factory.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include <iostream>

using namespace nepomuk;
// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(compute_scc)
{
    tool::container::ForwardStarGraphFactory factory;
    //
    // 0 - 1 -> 4 -> 6
    // |   |    |
    // 3 - 2    5
    auto graph = factory.allocate(7, 12);
    // 0
    factory.add_node(graph);
    factory.add_edge(graph, 1);
    factory.add_edge(graph, 3);
    // 1
    factory.add_node(graph);
    factory.add_edge(graph, 0);
    factory.add_edge(graph, 2);
    factory.add_edge(graph, 4);
    // 2
    factory.add_node(graph);
    factory.add_edge(graph, 1);
    factory.add_edge(graph, 3);
    // 3
    factory.add_node(graph);
    factory.add_edge(graph, 0);
    factory.add_edge(graph, 2);
    // 4
    factory.add_node(graph);
    factory.add_edge(graph, 6);
    factory.add_edge(graph, 5);
    // 5
    factory.add_node(graph);
    factory.add_edge(graph, 4);
    // 6
    factory.add_node(graph);

    BOOST_CHECK(factory.valid());

    auto components = algorithm::computeSCC(graph);
    BOOST_CHECK(components.size() == 3);
    BOOST_CHECK_EQUAL(components.component(0), components.component(1));
    BOOST_CHECK_EQUAL(components.component(0), components.component(2));
    BOOST_CHECK_EQUAL(components.component(0), components.component(3));
    BOOST_CHECK_EQUAL(components.component(4), components.component(5));
    BOOST_CHECK(components.component(0) != components.component(4));
    BOOST_CHECK(components.component(4) != components.component(6));
    BOOST_CHECK(components.component(0) != components.component(6));

    std::vector<std::size_t> sizes;
    for (std::size_t i = 0; i < components.size(); ++i)
        sizes.push_back(components.size(i));

    if (components.size() == 3)
    {
        std::sort(sizes.begin(), sizes.end());
        BOOST_CHECK_EQUAL(sizes[0], 1);
        BOOST_CHECK_EQUAL(sizes[1], 2);
        BOOST_CHECK_EQUAL(sizes[2], 4);
    }
}
