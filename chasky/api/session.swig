#if defined(SWIG)
%module chasky
%{
#include "chasky/api/session.h"
%}
%include "std_string.i"

#else

#include "chasky/api/session.h"

#endif

// python interface for Session
class session {
public:
  bool CreateGraph(const std::string& buf);

  bool StartExec();

  bool KillExec();

  bool DestroyGraph();

private:
  chasky::Session *session_;
};
