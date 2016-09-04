#include "chasky/core/framework/graph.h"

namespace chasky {

Graph::Graph(GraphDef &def, PostBox *postbox)
    : def_(def), node_lib_(postbox, &edge_lib_), postbox_(postbox) {
  CH_CHECK_OK(CreateNodes());
  CH_CHECK_OK(ConnectNodes());
}

Status Graph::CreateNodes() {
  Status status;
  CHECK(node_lib_.IsEmpty()) << "duplicate creating nodes";
  for (const auto &node_def : def_.nodes()) {
    LOG(INFO) << strings::Printf("create node: %s[%s]", node_def.name().c_str(),
                                 node_def.signature().c_str());
    CH_CHECK_OK(node_lib_.RegisterCreate(node_def.name(), node_def));
  }
  return status;
}

Status Graph::ConnectNodes() {
  Status status;
  CH_STEST_RETURN2(!node_lib_.IsEmpty(), error::UNKNOWN,
                   "no nodes are found in graph, create edges failed.");
  CHECK(edge_lib_.IsEmpty()) << "duplicate create edges";

  for (auto &e : *def_.mutable_edges()) {
    auto sign = EdgeLib::CreateKey(e);
    e.set_signature_(sign);
    edge_lib_.Register(sign);
  }

  return status;
}

} // namespace chasky
