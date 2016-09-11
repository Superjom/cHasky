%module chasky_engine
%include "std_string.i"

%{
#include "chasky/api/session.h"
%}


%ignore SessionPrivate;

using namespace std;

%include "session.h"

%rename(get_version) GetVersion;
