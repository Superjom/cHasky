#ifndef CHASKY_CORE_RUNTIME_GRAPH_BUILDER_H_
#define CHASKY_CORE_RUNTIME_GRAPH_BUILDER_H_
// session.h
// Build a compute graph for client, parse proto and fill in attributes in def
// level.
#include "chasky/core/common/status.h"
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/graph.h"
namespace chasky {

// Build a runtime computational graph from definition.
class GraphBuilder {
public:
  GraphBuilder() {}
  GraphBuilder(const GraphDef &graph_def)
      : graph_def_(graph_def), graph_(Graph::Create()) {}

  // Build a graph from protobuf definition
  Status Build();

  // Human readable debug information
  std::string debug_string() const;

protected:
  // Create all the nodes along with the corresponding functions
  Status CreateNodes();

  // Connect the nodes and build a computational graph
  Status ConnectNodes();

private:
  GraphDef graph_def_;
  std::unique_ptr<Graph> graph_;
  // NOTE All the nodes should be alive until graph is destroyed
  std::unordered_map<std::string, std::unique_ptr<Node> > nodes_;
};

}
#endif
