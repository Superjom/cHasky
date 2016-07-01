#include <cstdio>
#include "chasky/core/runtime/graph_builder.h"
#include "chasky/core/framework/func.h"
namespace chasky {

Status GraphBuilder::Build() {
  CH_CHECK_OK(CreateNodes());
  CH_CHECK_OK(ConnectNodes());
  return Status();
}

Status GraphBuilder::CreateNodes() {
  CHECK(nodes_.empty()) << "duplicate creating nodes";
  auto status = Status();
  // create nodes
  for (const auto &node_def : graph_def_.nodes()) {
    LOG(INFO) << strings::Printf("create node: %s[%s]", node_def.name().c_str(),
                                 node_def.op().c_str());
    auto node = Node::Create(node_def);
    if (!nodes_.insert({node_def.name(), std::move(node)}).second) {
      status.Update(Status{error::UNKNOWN,
                           strings::Printf("session insert node [%s] failed.",
                                           node_def.name().c_str())});
    }
  }
  return status;
}

// NOTE Should be called after CreateNodes
Status GraphBuilder::ConnectNodes() {
  CHECK(!nodes_.empty()) << "should create nodes first.";
  const size_t BUF_SIZE;
  char node1_name[BUF_SIZE];
  char node2_name[BUF_SIZE];
  char arg_name[BUF_SIZE];
  auto status = Status();

  for (const auto &node_def : graph_def_.nodes()) {
    std::string cur_node_name = node_def.name();
    for (const std::string &input : node_def.input()) {
      std::sscanf(input.c_str(), "%s:%s", node1_name, arg_name);
      snprintf(node2_name, cur_node_name.size(), "%s", cur_node_name.c_str());
      auto node1 = graph_->Nodes().find(node1_name);
      auto node2 = graph_->Nodes().find(node_def.name());
#define TEST_NODE_EXISTS(NODE)                                                        \
  if (NODE == graph_->Nodes().end()) {                                         \
    status.Update(Status(                                                      \
        error::OUT_OF_RANGE,                                                   \
        strings::Printf("node %s is not created, but called.", NODE##_name))); \
  }
      CHECK_NODE_EXISTS(node1);
      CHECK_NODE_EXISTS(node2);

#undef TEST_NODE_EXISTS

      // TODO CHECK node exists
      auto edge = std::unique_ptr<Edge>(new Edge(node1, node2, arg_name));
      // TODO change tostring to StringPiece, implement hash function
      graph_->mutable_edges()->emplace(edge->Signature().ToString(), edge);
    }
  }
}

}
