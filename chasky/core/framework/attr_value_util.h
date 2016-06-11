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
  AttrDefBuilder() {}
  AttrDefBuilder(const std::string &name) { def_.set_name(name); }

  AttrDefBuilder &Name(const std::string& name) {
    CHECK(def_.name().empty()) << "duplicate set type";
    def_.set_name(name);
    return *this;
  }

  AttrDefBuilder &Type(const std::string &type) {
    CHECK(def_.type().empty()) << "duplicate set type";
    def_.set_type(type);
    return *this;
  }

  AttrDefBuilder &Doc(const std::string &doc) {
    CHECK(def_.doc().empty()) << "duplicate set doc";
    def_.set_doc(doc);
    return *this;
  }

  // The string type value will be transformed to the coresspondding types.
  // For Example:
  // 'strs' -> bytes
  // '32' -> 32
  // '1,2,3,4' -> list(1, 2, 3, 4)
  AttrDefBuilder &Value(const std::string &val) {
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
