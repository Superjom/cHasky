#ifndef CHASKY_CORE_RUNTIME_SESSION_H_
#define CHASKY_CORE_RUNTIME_SESSION_H_
#include <memory>

#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/graph.h"
// #include "chasky/core/runtime/graph_builder.h"
// Session implemention for python client
namespace chasky {

// Session is an interface for python client to control computation graph's
// exectuation. Including creating, destroying graph, start computation and
// kill the session.
// It hold all the resources for the graph, and manage their lifetime.
class Session {
public:
  // Session();
  static std::unique_ptr<Session> Create(const std::string &name) {
    return std::unique_ptr<Session>(new Session(name));
  }

  Session(const std::string &name);

  Status CreateGraph(GraphDef &def);

  Status StartExec();

  Status Compute(std::vector<ArgumentDef> &inputs);

  Status KillExec();
  // @name: graph's name
  Status DestroyGraph();

  Graph &graph() { return *graph_; }

  PostBox &postbox() { return postbox_; }

  ~Session() {
    DLOG(INFO) << "session desc ...";
    postbox_.Abort();
    graph_->ServiceThreadJoin();
  }

private:
  std::string name_;
  std::shared_ptr<Graph> graph_;
  PostBox postbox_;
  // std::unique_ptr<GraphBuilder> graph_builder_;
};

// Get available functions
std::vector<std::string> AvailableFunctions();

std::string FunctionLibDebugString();
} // namespace chasky

#endif
