#include <cstdio>
#include "chasky/core/runtime/graph_builder.h"
#include "chasky/core/framework/function.h"
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
                                 node_def.signature().c_str());
    auto node = Node::Create(node_def, graph_.get());
    if (!nodes_.insert(std::make_pair(node_def.name(), std::move(node)))
             .second) {
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
  Status status;

  for (auto &e : *graph_def_.mutable_edges()) {
    auto src_node = graph_->Nodes().find(e.src_node());
    auto trg_node = graph_->Nodes().find(e.trg_node());

#define TEST_NODE_EXISTS(NODE, NODE_NAME)                                      \
  if (NODE == graph_->Nodes().end()) {                                         \
    status.Update(                                                             \
        Status(error::OUT_OF_RANGE,                                            \
               strings::Printf("node [%s] is not created, but called.",        \
                               NODE_NAME.c_str())));                           \
  }
    TEST_NODE_EXISTS(src_node, e.src_node());
    TEST_NODE_EXISTS(trg_node, e.trg_node());
#undef TEST_NODE_EXISTS

    auto edge = std::make_shared<Edge>(src_node->second.get(), e.src_arg(),
                                       trg_node->second.get(), e.trg_arg());
    *e.mutable_signature_() = edge->Signature().tostring();
    graph_->RegisterEdge(edge->Signature(), edge);
    // graph_->mutable_edges()->insert(
    //     std::make_pair(edge->Signature().tostring(), std::move(edge)));
  }

  return status;
}

} // namespace chasky
