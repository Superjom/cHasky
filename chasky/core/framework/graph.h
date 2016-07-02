#ifndef CHASKY_CORE_FRAMEWORK_GRAPH_H_
#define CHASKY_CORE_FRAMEWORK_GRAPH_H_
#include <unordered_map>
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/graph.pb.h"
namespace chasky {

class Graph {
public:

  static std::unique_ptr<Graph> Create();

  // getters
  const decltype(nodes_) Nodes() const { return nodes_; }
  decltype(nodes_) *mutable_nodes() { return &nodes_; }
  const decltype(edges_) Edges() const { return edges_; }
  const decltype(edges_) *mutable_edges() const {
    return &edges_;
  }

private:
  std::unordered_map<std::string, std::unique_ptr<Node> > nodes_;
  std::unordered_map<std::string, std::unique_ptr<Edge> > edges_;
};
}
#endif
