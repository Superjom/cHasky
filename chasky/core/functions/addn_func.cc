#include "chasky/core/framework/types.pb.h"
#include "chasky/core/common/register.h"
#include "chasky/core/common/status.h"
#include "chasky/core/common/eigen_vector.h"
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/functions/AddN.h"
#include "chasky/core/framework/argument.pb.h"

namespace chasky {
namespace functions {

template <typename T>
Status AddN<T>::FromDef(
    const FunctionDef &func_def,
    const ::google::protobuf::Map<::std::string, ::chasky::AttrValue> &attrs) {
  Status status;
  // TODO
  auto def = func_def;
  // Check definition, attributes like `dim` and `dtype` should be set.
  auto dtype_it = attrs.find("dtype");
  CH_STEST_RETURN(dtype_it != attrs.end(), error::INVALID_ARGUMENT,
                  "node's attribute [dtype] should be set");

  auto dim_it = attrs.find("dim");
  CH_STEST_RETURN(dim_it != attrs.end(), error::INVALID_ARGUMENT,
                  "node's attribute [dim] should be set");

  // test dtype value
  dtype_ = dtype_it->second.dtype_val();
  CH_STEST_RETURN(dtype_ != CH_INVALID, error::INVALID_ARGUMENT,
                  "value of node's attribute [dtype] should be set");

  // test dim value
  dim_ = dim_it->second.int64_val();
  CH_STEST_RETURN(dim_ != 0, error::INVALID_ARGUMENT,
                  "value of node's attribute [dim] should be set");

  // Set Argument's Input and Output dtype
  for (auto &x : *def.mutable_inputs()) {
    DLOG(INFO) << "prepare input's shape";
    if (x.dtype() == CH_INVALID) {
      x.set_dtype(dtype_);
    }
    if (x.shape().width() == 0) {
      x.mutable_shape()->set_width(dim_);
    }
  }
  for (auto &x : *def.mutable_outputs()) {
    DLOG(INFO) << "prepare output's shape";
    if (x.dtype() == CH_INVALID) {
      x.set_dtype(dtype_);
    }
    if (x.shape().width() == 0) {
      x.mutable_shape()->set_width(dim_);
    }
  }
  return status;
}

template <typename T> Status AddN<T>::InitFromProto(const FunctionDef &def) {
  UN_IMPLEMENTED

  Status status;
  CH_TEST_OR_UPDATE_STATUS(
      def.name() == "add_n", error::INVALID_ARGUMENT,
      strings::Printf("wrong function def [%s] is passed to AddN",
                      def.name().c_str()));
  // get each input
  // T *tmp_ptr = nullptr;
  return status;

  // check dims
}

template <typename T> void AddN<T>::CheckContext() {}

template <typename T>
Status AddN<T>::ForwardCompute(const std::vector<const Argument *> &args,
                               const std::vector<Argument *> *activations,
                               const FunctionDef &def) {
  Status status;
  // AddN has only 1 output

  // CHECK_EQ(activations->size(), 1UL);
  CH_STEST_RETURN2(activations->size() == 1UL, error::INVALID_ARGUMENT,
                   "activations' size should be only 1")
  T *output_ptr = nullptr;
  // The specific field's memory should be created by Node before.
  CHECK(activations->at(0)->ArgField()->get(&output_ptr));

  // only one input argument with a list of matrix
  std::vector<std::shared_ptr<T>> *inputs = nullptr;
  CHECK(args[0]->ArgField()->get(&inputs));
  LOG(INFO) << "output: " << output_ptr->Description();
  for (auto arg : *inputs) {
    LOG(INFO) << "input: " << arg->Description();
    output_ptr->AddWith(*arg);
  }
  return status;
}

// @previous_grad: should be a list
template <typename T>
Status AddN<T>::BackwardCompute(const std::vector<const Argument *> &x,
                                const Argument &grad,
                                const std::vector<Argument *> *previous_grad) {
  Status status;
  std::vector<std::shared_ptr<T>> *x_inputs, *previous_grad_list;
  T *grad_val;
  // Both x and previous_grad has one single argument with list data type.
  CHECK_EQ(x.size(), 1UL) << "x should have only 1 argument";
  CHECK_EQ(previous_grad->size(), 1UL)
      << "previous_grad should have only 1 argument";
  // Both x and previous_grad should have the same number of element in list
  // field.
  CHECK(x[0]->ArgField()->get(&x_inputs));
  CHECK(previous_grad->at(0)->ArgField()->get(&previous_grad_list));
  CHECK(grad.ArgField()->get(&grad_val));
  CHECK_EQ(x_inputs->size(), previous_grad_list->size())
      << "both x and previous_grad should have the same number of elements in "
         "list field ";
  CHECK_GT(x_inputs->size(), 0)
      << "both x and previous_grad should have at least one inputs";

  for (auto &pg : *previous_grad_list) {
    pg->CopyFrom(*grad_val);
  }
  return status;
}

REGISTER_FUNC(AddN, CH_MAT_FLOAT, AddN<math::CpuFloatMatrix>);
// REGISTER_FUNC(AddN, CH_MAT_DOUBLE, AddN<double>);

} // namespace functions
} // namespace chasky
