#ifndef TRANSIT_TOOL_CONTAINER_FORWARD_STAR_GRAPH_FACTORY_HPP_
#define TRANSIT_TOOL_CONTAINER_FORWARD_STAR_GRAPH_FACTORY_HPP_

#include "tool/container/errors.hpp"
#include "tool/container/forward_star_graph.hpp"

#include <cstddef>

#include <boost/assert.hpp>

namespace transit
{
namespace tool
{
namespace container
{

class ForwardStarGraphFactory
{
  public:
    // Needs to be called first, allocates the required nodes/edges in advance so we can connect
    // them with pointers appropriately
    template <typename external_node_type = ForwardStarZeroBaseClass,
              typename edge_data_class = ForwardStarZeroBaseClass>
    ForwardStarGraph<external_node_type, edge_data_class>
    allocate(std::size_t const number_of_nodes, std::size_t const number_of_edges);

    // Add a new node to the graph. Every node has to be added in succession with all its edges
    template <typename external_node_type, typename edge_data_class>
    std::size_t add_node(ForwardStarGraph<external_node_type, edge_data_class> &graph);

    // version of adding a node with data
    template <typename external_node_type, typename edge_data_class>
    std::size_t add_node(ForwardStarGraph<external_node_type, edge_data_class> &graph,
                         external_node_type const &node_data);

    // add an edge to the last added node
    template <typename external_node_type, typename edge_data_class>
    void add_edge(ForwardStarGraph<external_node_type, edge_data_class> &graph,
                  std::size_t const target);

    // add an edge with data to the last added node
    template <typename external_node_type, typename edge_data_class>
    void add_edge(ForwardStarGraph<external_node_type, edge_data_class> &graph,
                  std::size_t const target,
                  edge_data_class const &edge_data);

    bool valid() const { return required_nodes == 0 && required_edges == 0; }
  private:
    std::size_t required_nodes;
    std::size_t required_edges;
};

template <typename external_node_type, typename edge_data_class>
ForwardStarGraph<external_node_type, edge_data_class>
ForwardStarGraphFactory::allocate(std::size_t const number_of_nodes,
                                  std::size_t const number_of_edges)
{
    ForwardStarGraph<external_node_type, edge_data_class> graph;
    required_nodes = number_of_nodes;
    graph._nodes.resize(number_of_nodes + 1);
    graph._nodes.back()._offset = number_of_edges;

    required_edges = number_of_edges;
    graph._edges.reserve(number_of_edges);
    return graph;
}

template <typename external_node_type, typename edge_data_class>
std::size_t
ForwardStarGraphFactory::add_node(ForwardStarGraph<external_node_type, edge_data_class> &graph)
{
    if (!required_nodes)
        throw SizeError("Adding more nodes than allocated.");

    std::size_t node_index = graph._nodes.size() - required_nodes - 1;

    graph._nodes[node_index]._offset = graph._edges.size();

    --required_nodes;

    return node_index;
}

template <typename external_node_type, typename edge_data_class>
std::size_t
ForwardStarGraphFactory::add_node(ForwardStarGraph<external_node_type, edge_data_class> &graph,
                                  external_node_type const &node_data)
{
    // do not reduce, node already added without data
    auto const index = add_node(graph);
    graph._nodes[index] = typename ForwardStarGraph<external_node_type, edge_data_class>::node_type(
        graph._nodes[index]._offset, node_data);
    return index;
}

// add an edge to the last added node
template <typename external_node_type, typename edge_data_class>
void ForwardStarGraphFactory::add_edge(ForwardStarGraph<external_node_type, edge_data_class> &graph,
                                       std::size_t const target)
{
    if (!required_edges)
        throw SizeError("Adding more edges than allocated");

    graph._edges.push_back(
        typename ForwardStarGraph<external_node_type, edge_data_class>::edge_type());
    graph._edges.back()._target = graph._nodes.begin() + target;
    --required_edges;
}

template <typename external_node_type, typename edge_data_class>
void ForwardStarGraphFactory::add_edge(ForwardStarGraph<external_node_type, edge_data_class> &graph,
                                       std::size_t const target,
                                       edge_data_class const &edge_data)
{
    if (!required_edges)
        throw SizeError("Adding more edges than allocated");

    // create a new edge containing the data
    graph._edges.push_back(
        typename ForwardStarGraph<external_node_type, edge_data_class>::edge_type(
            graph._nodes.begin() + target, edge_data));
    --required_edges;
}

} // namespace container
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_CONTAINER_FORWARD_STAR_GRAPH_FACTORY_HPP_
