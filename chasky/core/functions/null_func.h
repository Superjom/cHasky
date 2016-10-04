/*
 * NullFunc.h
 *
 * Just for testing ovearall process, a function with nothing.
 */
#ifndef CHASKY_CORE_FUNCTIONS_NULL_FUNC_H_
#define CHASKY_CORE_FUNCTIONS_NULL_FUNC_H_
#include "chasky/core/framework/function.h"
#include "chasky/core/functions/function_def.h"

namespace chasky {
namespace functions {

REGISTER_FUNC_DEF(
    null,
    FunctionDefBuilder()
        .Name("null")
        // data type of input_list should be detemined in the runtime.
        .Input(ArgumentDefBuilder().Name("input").Type("float_mat").Finalize())
        // data type of output should be detemined in the runtime.
        .Output(
            ArgumentDefBuilder().Name("output").Type("float_mat").Finalize())
        .Finalize());

template <typename T> class null_func : public Function {
public:
  virtual Status FromDef(FunctionDef &func_def,
                         const Function::extra_attr_t &attrs) override {
    Status status;
    DLOG(INFO) << "NullFunc init from def";
    return status;
  }

  virtual Status InitFromProto(const FunctionDef &def) override {
    Status status;
    return status;
  }

  virtual Status ForwardCompute() override {
    Status status;
    return status;
  }

  Status BackwardCompute() override {
    Status status;
    return status;
  }

  virtual void CheckContext() override {}

private:
};

} // namespace functions
} // namespace chasky
#endif
