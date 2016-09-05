#ifndef CHASKY_API_SESSION_H_
#define CHASKY_API_SESSION_H_
#include <string>
#include <memory>
#include <unordered_map>

// Session implemention for python client

#define VERSION "0.0.1"

std::string GetVersion();

struct SessionPrivate;
// Session is an interface for python client to control computation graph's
// exectuation. Including creating, destroying graph, start computation and
// kill the session.
// It hold all the resources for the graph, and manage their lifetime.
class Session {
public:
  Session() {};

  // Session(const std::string &name);

  bool CreateGraph(const std::string &str_buf);

  bool StartExec();

  bool KillExec();
  // @name: graph's name
  bool DestroyGraph();

private:
  std::string name_;
  // std::unique_ptr<Graph> graph_;
  SessionPrivate* session_;
  // std::unique_ptr<Graph> graph_;
};

std::string FunctionLibDebugString();

// Return 0 for success, -1 for fail
// int CreateGraph(GraphDef *def);
#endif
