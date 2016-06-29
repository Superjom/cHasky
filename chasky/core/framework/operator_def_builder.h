#ifndef CHASKY_CORE_FRAMEWORK_OPERATOR_DEF_BUILDER_H_
#define CHASKY_CORE_FRAMEWORK_OPERATOR_DEF_BUILDER_H_
#include <string>
#include <unordered_map>
#include "chasky/core/common/status.h"
#include "chasky/core/framework/operator.pb.h"
namespace chasky {

// A helper class to specifing an operator's OperatorDef's content
// All the fields are defined by string. Different fields will parse string to
// different types.
class OperatorDefBuilder {
public:
  explicit OperatorDefBuilder() {}
  explicit OperatorDefBuilder(const std::string &name) { Name(name); }

  OperatorDefBuilder &Name(const std::string &x) {
    def_.set_name(x);
    return *this;
  }

  // Add an attribute once.
  // Assign an attr like this:
  //
  //   OperatorDefBuilder builder;
  //   builder.Attr(
  //     NewAttrDefBuilder().Name("attr1")
  //                        .Type("float")
  //                        .Finalize()
  //   )
  OperatorDefBuilder &Attr(const AttrDef &x) {
    // TODO replace copy assign with shared_ptr/unique_ptr here.
    *def_.add_attrs() = x;
    return *this;
  }

  // Add an input like:
  //
  // OperatorDefBuilder builder;
  // builder.Input(
  //     NewArgumentDefBuilder().Name("input1")
  //     .Type("float")
  //     .Finalize());
  OperatorDefBuilder &Input(const ArgumentDef &x) {
    *def_.add_inputs() = x;
    return *this;
  }

  // Add an output once.
  OperatorDefBuilder &Output(const ArgumentDef &x) {
    *def_.add_outputs() = x;
    return *this;
  }

  // Set one line short description
  OperatorDefBuilder &Summary(const std::string &x) {
    def_.set_summary(x);
    return *this;
  }

  // Add a detailed human-readable description
  OperatorDefBuilder &Doc(const std::string &x) {
    def_.set_doc(x);
    return *this;
  }

  // Fill empty attributes by inspecting from inputs and outputs
  OperatorDef &Finalize();

  std::string DebugString() const { return def_.DebugString(); }

private:
  OperatorDef def_;
};

class OperatorDefLibrary {
public:
  Status Register(const std::string &name, OperatorDef &&def);

  Status LookUp(const std::string &name, const OperatorDef *def);

  static OperatorDefLibrary &Instance() {
    static OperatorDefLibrary *x = new OperatorDefLibrary();
    return *x;
  }

  std::string DebugString() const;

  size_t Size() const;

private:
  std::unordered_map<std::string, OperatorDef> def_library_;
  ;
};

// Register operator definition like
// REGISTER_OP_DEF(op_name, OpDefBuilder().Name("op_naem")
//                                        .Input("src1")
//                                        .Input("src2")
//                                        .Output("out1")
//                                        .Attr("size:int32"))
#define REGISTER_OP_DEF(NAME, DEF)                                             \
  OperatorDefLibrary::Instance().Register(NAME, DEF);
}
#endif
