#ifndef CHASKY_API_SESSION_H_
#define CHASKY_API_SESSION_H_
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "chasky/core/framework/argument.pb.h"

// Session implemention for python client

#define VERSION "0.0.1"

using namespace std;

string GetVersion();

struct SessionPrivate;
// Session is an interface for python client to control computation graph's
// exectuation. Including creating, destroying graph, start computation and
// kill the session.
// It hold all the resources for the graph, and manage their lifetime.
class Session {
public:
  Session(){};

  // Session(const std::string &name);

  bool CreateGraph(const std::string &str_buf);

  bool StartExec();

  bool Compute(std::vector<std::string> &inputs);

  bool KillExec();
  // @name: graph's name
  bool DestroyGraph();

private:
  std::string name_;
  // std::unique_ptr<Graph> graph_;
  SessionPrivate *session_;
  // std::unique_ptr<Graph> graph_;
};

std::string FunctionLibDebugString();

// Return 0 for success, -1 for fail
// int CreateGraph(GraphDef *def);
#endif
