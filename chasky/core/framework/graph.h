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

  Status StartExec();

  Status Compute(std::vector<ArgumentDef> &inputs);

  string DebugString() const;

  void ServiceThreadJoin();

  ~Graph();

protected:
  Graph(GraphDef &def, PostBox *postbox);

  Status CreateNodes();

  Status ConnectNodes();

private:
  EdgeLib edge_lib_;
  NodeLib node_lib_;
  ParameterLib param_lib_;
  GraphDef def_;
  PostBox *postbox_;
  std::unique_ptr<DataProvider> data_provider_;

  std::unique_ptr<std::thread> service_thread_;
};

} // namespace chasky

#endif
