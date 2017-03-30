#ifndef TOOL_CONTAINER_FORWARD_STAR_GRAPH_HPP_
#define TOOL_CONTAINER_FORWARD_STAR_GRAPH_HPP_

#include <cstddef>
#include <iterator>
#include <vector>

#include <boost/assert.hpp>
#include <boost/range/iterator_range.hpp>

namespace transit
{
namespace tool
{
namespace container
{

class ForwardStarGraphFactory;
class ForwardStarZeroBaseClass
{
};

template <typename BaseClass> class AsForwardStarNode : public BaseClass
{
  public:
    AsForwardStarNode() : _offset(0) {}

    AsForwardStarNode(std::size_t const offset, BaseClass const &base)
        : BaseClass(base), _offset(offset)
    {
    }

    auto offset() const { return _offset; }
  private:
    auto self() { return static_cast<BaseClass *>(this); }
    auto self() const { return static_cast<const BaseClass *>(this); }

    std::size_t _offset;

    friend class ForwardStarGraphFactory;
};

// a class containing the edge data specified from the outside, as well as a node iterator pointing
// to the appropriate node in the graph
template <typename BaseClass, typename node_iterator> class AsForwardStarEdge : BaseClass
{
  public:
    auto target() const { return _target; };

    AsForwardStarEdge() {}

    AsForwardStarEdge(node_iterator const target, BaseClass const &base)
        : BaseClass(base), _target(target)
    {
    }

  private:
    node_iterator _target;
    friend class ForwardStarGraphFactory;
};

template <typename external_node_type, typename edge_data_class> class ForwardStarGraph
{
  public:
    friend class ForwardStarGraphFactory;
    // access into the graph
    using node_type = AsForwardStarNode<external_node_type>;
    using node_iterator = typename std::vector<node_type>::const_iterator;
    using node_iterator_range = boost::iterator_range<node_iterator>;

    using edge_type = AsForwardStarEdge<edge_data_class, node_iterator>;
    using edge_iterator = typename std::vector<edge_type>::const_iterator;
    using edge_iterator_range = boost::iterator_range<edge_iterator>;

    node_iterator_range nodes() const;

    // translating from node to id
    node_iterator node(std::size_t const offset) const;
    // and back
    std::size_t offset(node_iterator const node) const;
    std::size_t offset(node_type const &node) const;

    // access to edges (all / iterator / node)
    edge_iterator_range edges() const;
    edge_iterator_range edges(node_iterator const itr) const;
    edge_iterator_range edges(node_type const &node) const;

    // relates to nodes, not edges
    bool empty() const;
    // number of nodes
    std::size_t size() const;

  private:
    // make constructable only by factory
    ForwardStarGraph();

    std::vector<node_type> _nodes;
    std::vector<edge_type> _edges;
};

// default adjacency graph, only containing offsets / edge.target()
using AdjacencyGraph = ForwardStarGraph<ForwardStarZeroBaseClass, ForwardStarZeroBaseClass>;

template <typename external_node_type, typename edge_data_class>
ForwardStarGraph<external_node_type, edge_data_class>::ForwardStarGraph()
{
}

template <typename external_node_type, typename edge_data_class>
typename ForwardStarGraph<external_node_type, edge_data_class>::node_iterator_range
ForwardStarGraph<external_node_type, edge_data_class>::nodes() const
{
    // don't access the sentinel
    return boost::make_iterator_range(_nodes.begin(), _nodes.end() - 1);
}

template <typename external_node_type, typename edge_data_class>
typename ForwardStarGraph<external_node_type, edge_data_class>::node_iterator
ForwardStarGraph<external_node_type, edge_data_class>::node(std::size_t const offset) const
{
    return _nodes.begin() + offset;
}

template <typename external_node_type, typename edge_data_class>
typename ForwardStarGraph<external_node_type, edge_data_class>::edge_iterator_range
ForwardStarGraph<external_node_type, edge_data_class>::edges() const
{
    return boost::make_iterator_range(_edges.begin(), _edges.end());
}

template <typename external_node_type, typename edge_data_class>
typename ForwardStarGraph<external_node_type, edge_data_class>::edge_iterator_range
ForwardStarGraph<external_node_type, edge_data_class>::edges(node_iterator const itr) const
{
    BOOST_ASSERT(std::distance(_nodes.begin(), itr) >= 0 &&
                 static_cast<std::size_t>(std::distance(_nodes.begin(), itr) + 1) < _nodes.size());

    // use the sentinel to find the full range
    return boost::make_iterator_range(_edges.begin() + itr->offset(),
                                      _edges.begin() + (itr + 1)->offset());
}

template <typename external_node_type, typename edge_data_class>
typename ForwardStarGraph<external_node_type, edge_data_class>::edge_iterator_range
ForwardStarGraph<external_node_type, edge_data_class>::edges(node_type const &node) const
{
    auto const offset = &node - _nodes.data();
    BOOST_ASSERT(static_cast<std::size_t>(offset + 1) < _nodes.size());
    return edges(_nodes.begin() + offset);
}

template <typename external_node_type, typename edge_data_class>
bool ForwardStarGraph<external_node_type, edge_data_class>::empty() const
{
    return _nodes.empty();
}

template <typename external_node_type, typename edge_data_class>
std::size_t ForwardStarGraph<external_node_type, edge_data_class>::size() const
{
    return _nodes.size() - 1;
}

template <typename external_node_type, typename edge_data_class>
std::size_t
ForwardStarGraph<external_node_type, edge_data_class>::offset(node_iterator const node) const
{
    auto const offset = std::distance(_nodes.begin(), node);
    BOOST_ASSERT(offset >= 0 && static_cast<std::size_t>(offset + 1) < _nodes.size());
    return static_cast<std::size_t>(offset);
}

template <typename external_node_type, typename edge_data_class>
std::size_t
ForwardStarGraph<external_node_type, edge_data_class>::offset(node_type const &node) const
{
    auto const offset = &node - _nodes.data();
    BOOST_ASSERT(offset >= 0 && offset + 1 < _nodes.size());
    return static_cast<std::size_t>(offset);
}

} // namespace container
} // namespace tool
} // namespace transit

#endif // TOOL_CONTAINER_FORWARD_STAR_GRAPH_HPP_
