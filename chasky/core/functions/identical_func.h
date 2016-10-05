/*
 * IdenticalFunc.h
 *
 * Same to PaddlePaddle's IdentityProjection, outputs equals inputs.
 */
#ifndef CHASKY_CORE_FUNCTIONS_IDENTICAL_FUNC_H_
#define CHASKY_CORE_FUNCTIONS_IDENTICAL_FUNC_H_
#include "chasky/core/common/register.h"
#include "chasky/core/framework/function.h"
#include "chasky/core/functions/function_def.h"
#include "chasky/core/framework/attr_value_util.h"
#include "chasky/core/framework/function_def_builder.h"
#include "chasky/core/framework/argument_def_builder.h"

namespace chasky {
namespace functions {

REGISTER_FUNC_DEF(
    identical_func,
    FunctionDefBuilder()
        .Name("identical_func")
        // data type of input_list should be detemined in the runtime.
        .Input(ArgumentDefBuilder().Name("input").Type("float_mat").Finalize())
        // data type of output should be detemined in the runtime.
        .Output(
            ArgumentDefBuilder().Name("output").Type("float_mat").Finalize())
        .Attr(AttrDefBuilder()
                  .Name("dim")
                  .Type("int64")
                  .Doc("matrix dimention")
                  .Finalize())
        .Attr(AttrDefBuilder()
                  .Name("batch_size")
                  .Type("int64")
                  .Doc("size of the input batch")
                  .Finalize())
        .Finalize());

template <typename T> class identical_func : public Function {
public:
  virtual Status FromDef(FunctionDef &func_def,
                         const Function::extra_attr_t &attrs) override {
    Status status;
    DLOG(INFO) << "IdenticalFunc init from def";

    auto it = attrs.find("dim");
    CH_STEST_RETURN2(it != attrs.end(), error::INVALID_ARGUMENT,
                     "attribute dim is needed in node definition");
    dim_ = it->second.int64_val();
    DLOG(INFO) << "set attr dim to " << dim_;

    it = attrs.find("batch_size");
    CH_STEST_RETURN2(it != attrs.end(), error::INVALID_ARGUMENT,
                     "attribute batch_size is needed in node definition");
    batch_size_ = it->second.int64_val();
    DLOG(INFO) << "set attr batch_size to " << batch_size_;

    CHECK_GT(dim_, 0UL);
    CHECK_GT(batch_size_, 0UL);

    // fill empty attributes in function definition according to node
    // definition.
    def_ = &func_def;
    for (auto &def : *def_->mutable_inputs()) {
      def.mutable_shape()->set_width(dim_);
      def.mutable_shape()->set_height(batch_size_);
    }
    for (auto &def : *def_->mutable_outputs()) {
      def.mutable_shape()->set_width(dim_);
      def.mutable_shape()->set_height(batch_size_);
    }
    return status;
  }

  virtual Status InitFromProto(const FunctionDef &def) override {
    Status status;
    return status;
  }

  virtual Status ForwardCompute() override {
    DLOG(INFO) << "in forward function";
    Status status;
    CHECK_EQ(Inputs().size(), 1UL);
    CHECK_EQ(Outputs().size(), 1UL);
    if (!Inputs()[0]) {
      DLOG(WARNING) << "input nullptr, stop service";
      return status;
    }
    CHECK(Outputs()[0]) << "output is null1";
    CHECK(Outputs()[0]->ArgField()) << "output is null";
    *Outputs()[0]->ArgField()->float_mat_val->MatPtr() =
        *Inputs()[0]->ArgField()->float_mat_val->MatPtr();
    return status;
  }

  Status BackwardCompute() override {
    Status status;
    return status;
  }

  virtual void CheckContext() override {}

private:
  int64_t dim_;
  int64_t batch_size_;
};

} // functions

} // namespace chasky
#endif
