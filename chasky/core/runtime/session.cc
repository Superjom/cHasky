#include "chasky/common/macros.h"
#include "chasky/core/framework/graph.h"
#include "chasky/core/runtime/session.h"
#include "chasky/core/framework/function.h"

namespace chasky {

Session::Session(const std::string &name) : name_(name) {}

Status Session::CreateGraph(GraphDef &def) {
  Status status;
  graph_ = Graph::Create(def, postbox_.get());
  return status;
}

Status Session::StartExec() {
  Status status;
  UN_IMPLEMENTED;

  return status;
}

Status Session::KillExec() {
  Status status;
  UN_IMPLEMENTED;

  return status;
}

Status Session::DestroyGraph() {
  Status status;
  UN_IMPLEMENTED;

  return status;
}

std::string FunctionLibDebugString() {
  return FunctionLibrary::Instance().DebugString();
}


} // namespace chasky
