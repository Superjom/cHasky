// Chasky project's API for swig
#ifndef CHASKY_API_CHASKY_API_H_
#define CHASKY_API_CHASKY_API_H_
#include <string>

// I am not familiar with SWIG. Just add some simple necessary APIs here.
struct SessionPrivate;

class Session {
public:
  bool CreateGraph(const std::string& buf);

  bool StartExec();

  bool KillExec();

  bool DestroyGraph();
private:
  SessionPrivate *session_;
};

#endif
