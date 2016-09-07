#ifndef CHASKY_CORE_FRAMEWORK_GRAPH_DEF_BUILDER_H_
#define CHASKY_CORE_FRAMEWORK_GRAPH_DEF_BUILDER_H_

#include "chasky/core/framework/graph.pb.h"

namespace chasky {
class GraphDefBuilder {
public:
  explicit GraphDefBuilder() {}

  GraphDefBuilder &Name(const std::string &x) {
    def_.set_name(x);
    return *this;
  }

  GraphDefBuilder &AddNode(const NodeDef &x) {
    *(def_.add_nodes()) = x;
    return *this;
  }

  GraphDefBuilder &AddEdge(const EdgeDef &x) {
    *(def_.add_edges()) = x;
    return *this;
  }

  GraphDef &Finalize() {
    // TODO add some check here.
    return def_;
  }

private:
  GraphDef def_;
};
} // namespace chasky
#endif
