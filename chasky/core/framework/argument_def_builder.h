#ifndef CHASKY_CORE_FRAMEWORK_ARGUMENT_DEF_BUILDER_H_
#define CHASKY_CORE_FRAMEWORK_ARGUMENT_DEF_BUILDER_H_
#include <string>
#include "chasky/core/framework/argument.pb.h"
namespace chasky {
// A helper class to help specifying an argument's ArgumentDef
class ArgumentDefBuilder {
public:
  ArgumentDefBuilder &Name(const std::string &x) {
    def_.set_name(x);
    return *this;
  }

  ArgumentDefBuilder &Type(const std::string &x) {
    def_.set_type(x);
    return *this;
  }

  ArgumentDefBuilder &Shape(size_t width, size_t height) {
    def_.mutable_shape()->set_width(width);
    def_.mutable_shape()->set_height(height);
    return *this;
  }

  ArgumentDefBuilder &Doc(const std::string &x) {
    def_.set_doc(x);
    return *this;
  }

  ArgumentDefBuilder &SetIsRef(bool x) {
    def_.set_is_ref(x);
    return *this;
  }

  const ArgumentDef &Finalize() { return def_; }

  std::string DebugString() const { return def_.DebugString(); }

private:
  ArgumentDef def_;
};

inline ArgumentDefBuilder NewArgumentDefBuilder() {
  ArgumentDefBuilder x;
  return x;
}

}
#endif
