#include <string>

#include "tool/container/errors.hpp"
#include "tool/container/forward_star_graph.hpp"
#include "tool/container/forward_star_graph_factory.hpp"

using namespace nepomuk::tool::container;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

struct Data
{
    int value;
};

BOOST_AUTO_TEST_CASE(throw_on_too_many_nodes)
{
    ForwardStarGraphFactory factory;
    auto graph = factory.allocate(2, 0);
    BOOST_CHECK_NO_THROW(factory.add_node(graph));
    BOOST_CHECK_NO_THROW(factory.add_node(graph));
    BOOST_CHECK_THROW(factory.add_node(graph), SizeError);
}

BOOST_AUTO_TEST_CASE(throw_on_too_many_edges)
{
    ForwardStarGraphFactory factory;
    auto graph = factory.allocate(2, 2);
    BOOST_CHECK_NO_THROW(factory.add_node(graph));
    BOOST_CHECK_NO_THROW(factory.add_edge(graph, 1));
    BOOST_CHECK_NO_THROW(factory.add_node(graph));
    BOOST_CHECK_NO_THROW(factory.add_edge(graph, 0));
    BOOST_CHECK_THROW(factory.add_edge(graph, 1), SizeError);
}

BOOST_AUTO_TEST_CASE(throw_on_too_many_nodes_data)
{
    ForwardStarGraphFactory factory;
    auto graph = factory.allocate<Data, Data>(2, 0);
    BOOST_CHECK_NO_THROW(factory.add_node(graph, {1}));
    BOOST_CHECK_NO_THROW(factory.add_node(graph, {2}));
    BOOST_CHECK_THROW(factory.add_node(graph, {1}), SizeError);
}

BOOST_AUTO_TEST_CASE(throw_on_too_many_edges_data)
{
    ForwardStarGraphFactory factory;
    auto graph = factory.allocate<Data, Data>(2, 2);
    BOOST_CHECK_NO_THROW(factory.add_node(graph, {1}));
    BOOST_CHECK_NO_THROW(factory.add_edge(graph, 1, {2}));
    BOOST_CHECK_NO_THROW(factory.add_node(graph, {2}));
    BOOST_CHECK_NO_THROW(factory.add_edge(graph, 0, {3}));
    BOOST_CHECK_THROW(factory.add_edge(graph, 1, {42}), SizeError);
}
