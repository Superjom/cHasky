%module chasky_engine
%include "std_string.i"
%include "std_vector.i"

%{
#include "chasky/api/session.h"
%}

namespace std {
   %template(IntVector) vector<int>;
   %template(DoubleVector) vector<double>;
   %template(FloatVector) vector<float>;
   %template(StringVector) vector<string>;
};

%ignore SessionPrivate;

using namespace std;

%include "session.h"

%rename(get_version) GetVersion;
