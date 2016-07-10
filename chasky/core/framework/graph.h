#ifndef CHASKY_CORE_FRAMEWORK_GRAPH_H_
#define CHASKY_CORE_FRAMEWORK_GRAPH_H_
#include <unordered_map>
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/graph.pb.h"
namespace chasky {

class Graph {
public:
  static std::unique_ptr<Graph> Create();
  typedef std::unordered_map<std::string, std::unique_ptr<Node>> nodes_t;
  typedef std::unordered_map<std::string, std::unique_ptr<Edge>> edges_t;

  // getters
  const nodes_t &Nodes() const { return nodes_; }
  nodes_t *mutable_nodes() { return &nodes_; }
  const edges_t &Edges() const { return edges_; }
  edges_t *mutable_edges() { return &edges_; }

private:
  std::unordered_map<std::string, std::unique_ptr<Node>> nodes_;
  std::unordered_map<std::string, std::unique_ptr<Edge>> edges_;
};

} // namespace chasky
#endif
