#ifndef CHASKY_CORE_FRAMEWORK_GRAPH_H_
#define CHASKY_CORE_FRAMEWORK_GRAPH_H_
#include <unordered_map>

#include "chasky/core/framework/node.h"
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/runtime/edge_lib.h"
#include "chasky/core/runtime/node_lib.h"
#include "chasky/core/runtime/parameter_lib.h"

namespace chasky {
using std::string;

class Graph {
public:
  static std::unique_ptr<Graph> Create(GraphDef &def, PostBox *postbox) {
    return std::unique_ptr<Graph>(new Graph(def, postbox));
  }

  string DebugString() const;

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
};

} // namespace chasky

#endif
