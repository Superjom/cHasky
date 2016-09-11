#include "chasky/core/framework/graph.h"

namespace chasky {

Graph::Graph(GraphDef &def, PostBox *postbox)
    : data_provider_(
          DataProvider::Create(def.data_provider(), postbox, &edge_lib_)),
      node_lib_(postbox, &edge_lib_), postbox_(postbox), def_(def) {
  CH_CHECK_OK(CreateNodes());
  CH_CHECK_OK(ConnectNodes());
}

Status Graph::StartExec() {
  Status status;

  service_thread_ = std::unique_ptr<std::thread>(new std::thread([&] {
    for (auto &item : node_lib_.Nodes()) {
      auto &node = item.second;
      CH_CHECK_OK(node->StartService());
    }

    DLOG(INFO) << "begin to join all thread";

    for (auto &item : node_lib_.Nodes()) {
      DLOG(INFO) << "thread " << item.first << " join";
      item.second->ServiceThreadJoin();
    }
  }));

  return status;
}

Status Graph::Compute(std::vector<ArgumentDef> &inputs) {
  Status status;
  // fill dataprovider
  CH_CHECK_OK(data_provider_->Fill(inputs));
  return status;
}

Status Graph::CreateNodes() {
  Status status;
  LOG(INFO) << "Creating Nodes ...";
  CHECK(node_lib_.IsEmpty()) << "duplicate creating nodes";
  LOG(INFO) << "node.size: " << def_.nodes().size();
  for (const auto &node_def : def_.nodes()) {
    LOG(INFO) << strings::Printf("create node: %s[%s]", node_def.name().c_str(),
                                 node_def.signature().c_str());
    CH_CHECK_OK(node_lib_.RegisterCreate(node_def.name(), node_def));
  }
  DLOG(INFO) << "finish create nodes ...";
  return status;
}

Status Graph::ConnectNodes() {
  Status status;
  DLOG(INFO) << "begin to connect nodes ...";
  CH_STEST_RETURN2(!node_lib_.IsEmpty(), error::UNKNOWN,
                   "no nodes are found in graph, create edges failed.");
  CHECK(edge_lib_.IsEmpty()) << "duplicate create edges";

  for (auto &e : *def_.mutable_edges()) {
    auto sign = EdgeLib::CreateKey(e);
    DLOG(INFO) << "connect " << sign;
    e.set_signature_(sign);
    edge_lib_.Register(sign);
  }
  DLOG(INFO) << "finish connecting nodes ...";

  return status;
}

void Graph::ServiceThreadJoin() {
  if (service_thread_ && service_thread_->joinable()) {
    service_thread_->join();
  }
}

Graph::~Graph() {
  DLOG(INFO) << "graph desc";

  ServiceThreadJoin();
}

} // namespace chasky
