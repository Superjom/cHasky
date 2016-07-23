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

  AttrDefBuilder &Name(const std::string &name) {
    CHECK(def_.name().empty()) << "duplicate set type, the previous name is '"
                               << def_.name() << "'";
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
  AttrDefBuilder &Value(const std::string &val);

  // TODO(superjom) replace copy with pointer.
  // A mechanism is needed to make assign and copy cheaper.
  const AttrDef &Def() const { return def_; }

  const AttrDef &Finalize() const {
    // TODO add some check here.
    return Def();
  }

private:
  AttrDef def_;
};

// A helper function to support syntax like:
//
//     NewAttrDefBuilder().Name("name")
//                        .Type("float")
//                        .Doc("a description");
inline AttrDefBuilder NewAttrDefBuilder() {
  AttrDefBuilder x;
  return x;
}

inline void StringToValue(const StringPiece &str, int64_t *val) {
  *val = stoi(str.tostring(), nullptr, 10);
}

inline void StringToValue(const StringPiece &str, float *val) {
  *val = stof(str.tostring(), nullptr);
}

// inline void StringToValue(const StringPiece &str, DataType *val) {
//   if (str == "float_mat") {
//     *val = CH_MAT_FLOAT;
//   }
// }

// A helper class to filling in the value.
// NOTE the attr_value's definition should be built before filling in value.
// The client should know all the attributes of a single operator
class AttrValueBuilder {
public:
  // Set Attr
  AttrValueBuilder() {}
  // set Value
  AttrValueBuilder &Value(float x) {
    attr_value_.set_float_val(x);
    return *this;
  }
  AttrValueBuilder &Value(int64_t x) {
    attr_value_.set_int64_val(x);
    return *this;
  }
  AttrValueBuilder &Value(std::string x) {
    attr_value_.set_string_val(x);
    return *this;
  }
  AttrValueBuilder &Value(bool x) {
    attr_value_.set_bool_val(x);
    return *this;
  }
  AttrValueBuilder &Value(DataType x) {
    attr_value_.set_dtype_val(x);
    return *this;
  }
  AttrValue Finalize() const { return std::move(attr_value_); }
  // TODO add list value

private:
  AttrValue attr_value_;
};
}
#endif
