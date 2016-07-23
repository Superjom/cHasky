#include <iostream>
#include "chasky/core/framework/attr_value_util.h"
using namespace std;

namespace chasky {

void StringToValue(const StringPiece &str, DataType *dtype) {
  if (str == "float")
    *dtype = CH_FLOAT;
  else if (str == "int64")
    *dtype = CH_INT64;
  else if (str == "uint64")
    *dtype = CH_UINT64;
  else if (str == "string")
    *dtype = CH_STRING;
  else if (str == "float_mat")
    *dtype = CH_MAT_FLOAT;
  else if (str == "float_mat_list")
    *dtype = CH_MAT_FLOAT_LIST;
  else {
    LOG(FATAL) << "not supported type : " << str;
  }
}

AttrDefBuilder &AttrDefBuilder::Value(const std::string &val) {
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
