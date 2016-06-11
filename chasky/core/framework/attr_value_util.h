#ifndef CHASKY_CORE_FRAMEWORK_ATTR_VALUE_UTIL_H_
#define CHASKY_CORE_FRAMEWORK_ATTR_VALUE_UTIL_H_
#include "chasky/common/string_piece.h"
#include "chasky/core/framework/attr_value.pb.h"
namespace chasky {

void StringToValue(const StringPiece &str, int64_t *val);
void StringToValue(const StringPiece &str, float *val);
void StringToValue(const StringPiece &str, DataType *dtype);

// Attr is defined like:
// "attr1:type1"
// Attr is filled like:
// "attr1=value"
class AttrDefBuilder {
public:
  AttrDefBuilder(const std::string &name) { def_.set_name(name); }

  AttrDefBuilder &Type(const std::string &type) {
    def_.set_type(type);
    return *this;
  }

  AttrDefBuilder &Doc(const std::string &doc) {
    def_.set_description(doc);
    return *this;
  }

  // The string type value will be transformed to the coresspondding types.
  // For Example:
  // 'strs' -> bytes
  // '32' -> 32
  // '1,2,3,4' -> list(1, 2, 3, 4)
  AttrDefBuilder &Value(const std::string &val) {
    StringPiece value(val);
    // parse to value
    if (def_.type() == "string") {
      def_.mutable_value()->set_s(val);
    } else if (def_.type() == "int64") {
      int64_t v;
      StringToValue(value, &v);
      def_.mutable_value()->set_i(v);
    } else if (def_.type() == "float") {
      float v;
      StringToValue(value, &v);
      def_.mutable_value()->set_f(v);
    } else if (def_.type() == "dtype") {
      DataType v;
      StringToValue(value, &v);
      def_.mutable_value()->set_t(v);
    } else if (def_.type() == "list") {
      LOG(FATAL) << "NotImplemented";
    }

    return *this;
  }

  // TODO(superjom) replace copy with pointer.
  // A mechanism is needed to make assign and copy cheaper.
  const AttrDef &Def() const { return def_; }

private:
  AttrDef def_;
};

inline void StringToValue(const StringPiece &str, int64_t *val) {
  *val = stoi(str.tostring(), nullptr, 10);
}

inline void StringToValue(const StringPiece &str, float *val) {
  *val = stof(str.tostring(), nullptr);
}

inline void StringToValue(const StringPiece &str, DataType *dtype) {
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
#endif
