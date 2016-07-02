#include "chasky/common/strings.h"
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/function_def_builder.h"

namespace chasky {
using std::string;

std::string GenEdgeKey(const string &node_name, const string &task,
                       const string &arg_name) {
  return strings::Printf("%s:%s->%s", node_name.c_str(), task.c_str(),
                         arg_name.c_str());
}

std::string GenEdgeKey(const std::string input, const std::string &node2) {
  return strings::Printf("%s->%s", input.c_str(), node2.c_str());
}

Edge::Edge(const Node *src, const Node *trg, const std::string &arg)
    : signature_(
          GenEdgeKey(src->Name().tostring(), trg->Name().tostring(), arg)),
      src_(src), trg_(trg) {}

Node::Node(const NodeDef &def) : def_(def) {
  FunctionLibrary::FunctionCreatorType func_creator;
  CH_CHECK_OK(FunctionLibrary::Instance().LookUp(def.op(), &func_creator));
  // init function's definition by filling attributes from node's definition.
  // TODO much code here
  // TODO just add unittest
  CH_CHECK_OK(FunctionDefLibrary::Instance().LookUp(def.func(), &func_def_));

  // create an function
  func_ = func_creator();
  CH_CHECK_OK(func_->FromDef(func_def_, def.attr()));
}

std::unique_ptr<Node> Node::Create(const NodeDef &def) {
  std::unique_ptr<Node> node(new Node(def));
  return node;
}

Status Node::Compute(chasky::TaskType task) {
  if (task == FORWORD) {
    ForwardCompute();
  } else {
    BackwardCompute();
  }
  return Status();
}

Status Node::ForwardCompute() {
  Status status = func_->ForwardCompute();
  if (status.ok()) {
    for (auto &e : in_links_) {
      ReleaseEdge(&e);
    }
  }
  return status;
}

Status Node::ReleaseEdge(const Edge *edge) {}

Status Node::StartService() {}

Status Node::ConnectTo(Node *other, bool forward_or_bachward) {
  auto status = Status();
  Node *src = this;
  Node *trg = other;

  if (!forward_or_bachward) {
    src = other;
    trg = this;
  }

  return status;
}

Node::~Node() {
  if (service_thread_.joinable()) {
    service_thread_.join();
  }
}
}
