#ifndef CHASKY_CORE_FRAMEWORK_FUNCTION_DEF_BUILDER_H_
#define CHASKY_CORE_FRAMEWORK_FUNCTION_DEF_BUILDER_H_
#include <string>
#include <unordered_map>
#include "chasky/core/common/status.h"
#include "chasky/core/framework/function.pb.h"
namespace chasky {

// A helper class to specifing an function's FunctionDef's content
// All the fields are defined by string. Different fields will parse string to
// different types.
class FunctionDefBuilder {
public:
  explicit FunctionDefBuilder() {}
  explicit FunctionDefBuilder(const std::string &name) { Name(name); }

  FunctionDefBuilder &Name(const std::string &x) {
    def_.set_name(x);
    return *this;
  }

  // Add an attribute once.
  // Assign an attr like this:
  //
  //   FunctionDefBuilder builder;
  //   builder.Attr(
  //     NewAttrDefBuilder().Name("attr1")
  //                        .Type("float")
  //                        .Finalize()
  //   )
  FunctionDefBuilder &Attr(const AttrDef &x) {
    // TODO replace copy assign with shared_ptr/unique_ptr here.
    *def_.add_attrs() = x;
    return *this;
  }

  // Add an input like:
  //
  // FunctionDefBuilder builder;
  // builder.Input(
  //     NewArgumentDefBuilder().Name("input1")
  //     .Type("float")
  //     .Finalize());
  FunctionDefBuilder &Input(const ArgumentDef &x) {
    *def_.add_inputs() = x;
    return *this;
  }

  // Add an output once.
  FunctionDefBuilder &Output(const ArgumentDef &x) {
    *def_.add_outputs() = x;
    return *this;
  }

  // Set one line short description
  FunctionDefBuilder &Summary(const std::string &x) {
    def_.set_summary(x);
    return *this;
  }

  // Add a detailed human-readable description
  FunctionDefBuilder &Doc(const std::string &x) {
    def_.set_doc(x);
    return *this;
  }

  // Fill empty attributes by inspecting from inputs and outputs
  FunctionDef &Finalize();

  std::string DebugString() const { return def_.DebugString(); }

private:
  FunctionDef def_;
};

class FunctionDefLibrary {
public:
  Status Register(const std::string &name, FunctionDef &&def);

  Status LookUp(const std::string &name, const FunctionDef *def);

  static FunctionDefLibrary &Instance() {
    static FunctionDefLibrary *x = new FunctionDefLibrary();
    return *x;
  }

  std::string DebugString() const;

  size_t Size() const;

private:
  std::unordered_map<std::string, FunctionDef> def_library_;
  ;
};

// Register function definition like
// REGISTER_OP_DEF(op_name, OpDefBuilder().Name("op_naem")
//                                        .Input("src1")
//                                        .Input("src2")
//                                        .Output("out1")
//                                        .Attr("size:int32"))
#define REGISTER_OP_DEF(NAME, DEF)                                             \
  FunctionDefLibrary::Instance().Register(NAME, DEF);
}
#endif
