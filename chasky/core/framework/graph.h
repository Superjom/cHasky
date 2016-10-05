#ifndef CHASKY_CORE_FRAMEWORK_GRAPH_H_
#define CHASKY_CORE_FRAMEWORK_GRAPH_H_
#include <unordered_map>

#include "chasky/core/framework/node.h"
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/runtime/edge_lib.h"
#include "chasky/core/runtime/node_lib.h"
#include "chasky/core/runtime/parameter_lib.h"
#include "chasky/core/framework/data_provider.h"

namespace chasky {
using std::string;

class Graph {
public:
  static std::shared_ptr<Graph> Create(GraphDef &def, PostBox *postbox) {
    return std::unique_ptr<Graph>(new Graph(def, postbox));
  }

  // Let graph to study one batch of records.
  Status Compute(std::vector<ArgumentDef> &inputs);

  string DebugString() const;

  GraphDef &Def() { return def_; }

  // ~Graph();

protected:
  Graph(GraphDef &def, PostBox *postbox);

  Status CreateNodes();
  Status ConnectNodes();

private:
  GraphDef def_;
  PostBox *postbox_;
  EdgeLib edge_lib_;
  NodeLib node_lib_;
  ParameterLib param_lib_;
  std::unique_ptr<DataProvider> data_provider_;
  // nodes' forward compu
  std::vector<std::string> nodes_fwd_cpt_order_;
  // nodes' backward computation order
  std::vector<std::string> nodes_bwd_cpt_order_;
};

} // namespace chasky

#endif
