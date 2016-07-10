#include "chasky/common/macros.h"
#include "chasky/core/runtime/session.h"

namespace chasky {

Session::Session(const std::string &name) : name_(name) {}

Status Session::CreateGraph(const GraphDef &def) {
  Status status;

  graph_builder_.reset(new GraphBuilder(def));
  CH_CHECK_OK(graph_builder_->Build());
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

} // namespace chasky
