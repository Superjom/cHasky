#include <iostream>
#include "chasky/core/framework/attr_value_util.h"
using namespace std;

namespace chasky {

void StringToValue(const StringPiece &str, DataType *dtype) {
  if (str == "float")
    *dtype = CH_FLOAT;
  else if (str == "double")
    *dtype = CH_DOUBLE;
  else if (str == "int32")
    *dtype = CH_INT32;
  else if (str == "int64")
    *dtype = CH_INT64;
  else if (str == "uint32")
    *dtype = CH_UINT32;
  else if (str == "uint64")
    *dtype = CH_UINT64;
  else if (str == "string")
    *dtype = CH_STRING;
  else if (str == "vec_float")
    *dtype = CH_VEC_FLOAT;
  else if (str == "vec_double")
    *dtype = CH_VEC_DOUBLE;
  else if (str == "vec_int32")
    *dtype = CH_VEC_INT32;
}

}
