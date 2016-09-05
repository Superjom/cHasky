%module chasky

%include "std_string.i"
%{
#include "chasky/api/session.h"
%}

%ignore SessionPrivate;

%include "session.h"

%rename(get_version) GetVersion;
