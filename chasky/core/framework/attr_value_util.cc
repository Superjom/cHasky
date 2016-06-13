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

AttrDefBuilder& AttrDefBuilder::Value(const std::string &val) {
  // TODO check value field is empty
  StringPiece value(val);
  // parse to value
  if (def_.type() == "string") {
    def_.mutable_value()->set_string_val(val);
  } else if (def_.type() == "int64") {
    int64_t v;
    StringToValue(value, &v);
    def_.mutable_value()->set_int64_val(v);
  } else if (def_.type() == "float") {
    float v;
    StringToValue(value, &v);
    def_.mutable_value()->set_float_val(v);
  } else if (def_.type() == "dtype") {
    DataType v;
    StringToValue(value, &v);
    def_.mutable_value()->set_dtype_val(v);
  } else if (def_.type() == "list") {
    LOG(FATAL) << "NotImplemented";
  } else {
    // TODO(superjom) add a paser factory to support type-parser registery
    // Types should be checked in the registry time
    LOG(FATAL) << "Unsupported type " << def_.type();
  }

  return *this;
}


}
