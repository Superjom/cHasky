#ifndef CHASKY_API_SESSION_H_
#define CHASKY_API_SESSION_H_
#include <memory>
#include <unordered_map>
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/graph.h"
// Session implemention for python client
namespace chasky {

// Session is an interface for python client to control computation graph's
// exectuation. Including creating, destroying graph, start computation and
// kill the session.
// It hold all the resources for the graph, and manage their lifetime.
class Session {
public:
  Session(const std::string &name);

  Status CreateGraph(const GraphDef &def);

  Status StartExec();

  Status KillExec();

  // @name: graph's name
  Status DestroyGraph(const std::string &name);

private:
  std::string name_;
  std::unique_ptr<Graph> graph_;
};

// Return 0 for success, -1 for fail
int CreateGraph(GraphDef *def);
}
#endif
