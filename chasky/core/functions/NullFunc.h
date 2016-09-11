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

template <typename T> class NullFunc : public Function {
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

  virtual Status
  ForwardCompute(const std::vector<const Argument *> &args,
                 const std::vector<ArgumentPtr> *activation) override {
    Status status;
    return status;
  }

  Status
  BackwardCompute(const std::vector<const Argument *> &x, const Argument &grad,
                  const std::vector<Argument *> *previous_grad) override {

    Status status;
    return status;
  }

  virtual void CheckContext() override {}

private:
};

} // namespace functions
} // namespace chasky
#endif
