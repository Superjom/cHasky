#ifndef CHASKY_CORE_FRAMEWORK_GRAPH_H_
#define CHASKY_CORE_FRAMEWORK_GRAPH_H_
#include <unordered_map>
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/graph.pb.h"
namespace chasky {

class Graph {
public:
  typedef std::unordered_map<std::string, std::unique_ptr<Node>> nodes_t;
  typedef std::unordered_map<std::string, std::unique_ptr<Edge>> edges_t;
  typedef std::unordered_map<std::string, std::shared_ptr<Argument>> args_t;

  static std::unique_ptr<Graph> Create();

  // Get a parameter by name
  std::shared_ptr<Argument> Param(const std::string &name);

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
  std::unordered_map<std::string, std::unique_ptr<Node>> nodes_;
  std::unordered_map<std::string, std::unique_ptr<Edge>> edges_;
  // Model parameters, shared by all nodes in a graph.
  // Key are "%s:%s" % (node.name, param.name)
  // The parameters are registered by Nodes.
  std::unordered_map<std::string, std::shared_ptr<Argument>> params_;
};

} // namespace chasky
#endif
