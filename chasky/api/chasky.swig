%module chasky

%include "std_string.i"
%{
#include "chasky/api/chasky_api.h"
%}

%ignore SessionPrivate;

%include "chasky_api.h"

%rename(get_version) GetVersion;
