#include "chasky/common/macros.h"
#include "chasky/core/framework/graph.h"
#include "chasky/core/runtime/session.h"
#include "chasky/core/framework/function.h"

namespace chasky {

Session::Session(const std::string &name) : name_(name) {}

Status Session::CreateGraph(GraphDef &def) {
  Status status;
  graph_ = Graph::Create(def, &postbox_);
  return status;
}

Status Session::StartExec() {
  Status status;
  CH_CHECK_OK(graph_->StartExec());
  return status;
}

Status Session::Compute(std::vector<ArgumentDef> &inputs) {
  return graph_->Compute(inputs);
}

Status Session::KillExec() {
  Status status;
  UN_IMPLEMENTED;

  return status;
}

Status Session::DestroyGraph() {
  Status status;

  return status;
}

std::string FunctionLibDebugString() {
  return FunctionLibrary::Instance().DebugString();
}

} // namespace chasky
