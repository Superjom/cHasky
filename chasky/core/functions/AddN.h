/*
 * AddN.h
 * Created Date: 2016-06-01
 * Created by Superjom.
 */
#ifndef CHASKY_CORE_FUNCTIONS_ADDN_H_
#define CHASKY_CORE_FUNCTIONS_ADDN_H_
#include "chasky/core/framework/function.h"
#include "chasky/core/functions/function_def.h"
namespace chasky {
namespace functions {

// AddN function adds `N` vectors and output a single vector
// All the inputs should be the same type, and same shape.
// For arguments, width is layer's size, height is batch size.
// User defines each argument type in function_def, and impement
// the function according to the corresponding function_def.
template <typename T> class AddN : public Function {
public:
  virtual Status FromDef(const FunctionDef &func_def,
                         const Function::extra_attr_t &attrs) override;

  // Init following attributes from attributes of functioin's definition.
  //     dim_
  virtual Status InitFromProto(const FunctionDef &def) override;

  // activation = sum(arguments)
  virtual Status ForwardCompute(const std::vector<const Argument *> &args,
                                const std::vector<Argument *> *activation,
                                const FunctionDef &def) override;

  // previous_grad += (this) grad
  // @x: list
  //   each element is a previous node's activation
  // @grad: matrix
  //   current node's grad
  // @previous_grad: list
  //   each element is a previous node's grad
  Status BackwardCompute(const std::vector<const Argument *> &x,
                         const Argument &grad,
                         const std::vector<Argument *> *previous_grad) override;

  // TODO remove this api?
  virtual void CheckContext() override;

  size_t Dim() const { return dim_; }
  DataType Dtype() const { return dtype_; }

private:
  // The dimention of each input. All the inputs should have the same dimention
  // Dimention is each matrix's width, height is batch size.
  size_t dim_;
  DataType dtype_;
};

} // namespace functions

} // namespace chasky
#endif
