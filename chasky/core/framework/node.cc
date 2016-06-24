#include "chasky/common/strings.h"
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/operator.h"

namespace chasky {
using std::string;
std::string GenArgKey(const string &node_name, const string &task,
                      const string &arg_name) {
  return strings::Printf("%s:%s:%s", node_name.c_str(), task.c_str(),
                         arg_name.c_str());
}

Node::Node(const NodeDef& def) {
  OperatorLibrary::OperatorCreatorType op_creator;
  CH_CHECK_OK(OperatorLibrary::Instance().LookUp(def.op(), &op_creator));
  // create an operator
  auto op = op_creator();
  // init operator's definition by filling attributes from node's definition.
  // TODO much code here
  // TODO just add unittest
}

std::unique_ptr<Node> Node::Create(const NodeDef& def) {
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
  Status status = op_->ForwardCompute();
  if (status.ok()) {
    for (auto &e : in_links_) {
      ReleaseEdge(&e);
    }
  }
  return status;
}

Status Node::ReleaseEdge(const Edge *edge) {
  string arg_key;
  if (cur_task_ == FORWORD) {
    arg_key = GenArgKey(op_->Def().name(), "forward", edge->activation.Name());
  } else {
    arg_key = GenArgKey(op_->Def().name(), "backward", edge->gradient.Name());
  }
  for (auto ep : edge->endpoints) {
    if (cur_task_ == FORWORD) {
      ep->mutable_exec_context()->AddInput(arg_key, edge->activation);
    } else {
      ep->mutable_exec_context()->AddGrad(arg_key, edge->gradient);
    }
  }
}

Status Node::StartService() {
  auto handler = [this] {
    while (exec_context_.KeepRunning()) {
      if (cur_task_ == FORWORD) {
        exec_context_.WaitUntilForwardReady();
      } else {
        exec_context_.WaitUntilBackwardReady();
      }
      Compute(cur_task_);
    }
  };

  std::thread t(handler);
  service_thread_ = std::move(t);

  return Status();
}

Node::~Node() {
  if (service_thread_.joinable()) {
    service_thread_.join();
  }
}
}
