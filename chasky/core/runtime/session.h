#ifndef CHASKY_CORE_RUNTIME_SESSION_H_
#define CHASKY_CORE_RUNTIME_SESSION_H_
#include <memory>
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/graph.h"
#include "chasky/core/runtime/graph_builder.h"
// Session implemention for python client
namespace chasky {

// Session is an interface for python client to control computation graph's
// exectuation. Including creating, destroying graph, start computation and
// kill the session.
// It hold all the resources for the graph, and manage their lifetime.
class Session {
 public:
  Session();

  Session(const std::string &name);

  Status CreateGraph(const GraphDef &def);

  Status StartExec();

  Status KillExec();
  // @name: graph's name
  Status DestroyGraph();

 private:
  std::string name_;
  //std::unique_ptr<Graph> graph_;
  std::unique_ptr<GraphBuilder> graph_builder_;
};


}
#endif
