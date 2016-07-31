#ifndef CHASKY_CORE_FRAMEWORK_GRAPH_H_
#define CHASKY_CORE_FRAMEWORK_GRAPH_H_
#include <unordered_map>
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/graph.pb.h"
namespace chasky {
class Graph;
using graph_ptr_t = std::shared_ptr<Graph>;

class Graph {
public:
  typedef std::unordered_map<std::string, node_ptr_t> nodes_t;
  typedef std::unordered_map<std::string, edge_ptr_t> edges_t;
  typedef std::unordered_map<std::string, std::shared_ptr<Argument>> args_t;

  static std::unique_ptr<Graph> Create();

  // Get a parameter by name
  std::shared_ptr<Argument> Param(const std::string &name);

  // Get all edges point to target Node identified by `signature`.
  // The `signature` should be format like "{node}:{arg_name}"
  Status GetEdgeByTarget(const std::string &signature,
                         std::vector<edge_ptr_t> *edges);

  // Get all edges point from Source Node which identified by `signature`.
  Status GetEdgesBySource(const std::string &signature,
                          std::vector<edge_ptr_t> *edges);

  // Register a parameter by name and definiton.
  // NOTE not thread-safe.
  Status RegisterParameter(const std::string &name, const ArgumentDef &def) {
    Status status;
    auto it = params_.find(name);
    if (it != params_.end()) {
      status.Update(error::INVALID_ARGUMENT,
                    strings::Printf("duplicate register parameter called %s",
                                    name.c_str()));
    } else if (!params_.insert({name, std::make_shared<Argument>(&def)})
                    .second) {
      status.Update(error::UNKNOWN,
                    strings::Printf("parameter %s insert error", name.c_str()));
    }
    return status;
  }

  // getters
  const nodes_t &Nodes() const { return nodes_; }
  nodes_t *mutable_nodes() { return &nodes_; }
  const edges_t &Edges() const { return edges_; }
  edges_t *mutable_edges() { return &edges_; }

private:
  std::unordered_map<std::string, node_ptr_t> nodes_;
  std::unordered_map<std::string, edge_ptr_t> edges_;
  // Model parameters, shared by all nodes in a graph.
  // Key are "%s:%s" % (node.name, param.name)
  // The parameters are registered by Nodes.
  std::unordered_map<std::string, std::shared_ptr<Argument>> params_;
};

} // namespace chasky
#endif
