/*
 * FullyConnectedFunc.h
 *
 * Same to PaddlePaddle's fc layer, multi-layer neural networks.
 */
#ifndef CHASKY_CORE_FUNCTIONS_FULLY_CONNECTED_FUNC_H_
#define CHASKY_CORE_FUNCTIONS_FULLY_CONNECTED_FUNC_H_
#include "chasky/core/framework/function.h"
#include "chasky/core/functions/function_def.h"

namespace chasky {
namespace functions {

template <typename T> class FullyConnectedFunc : public Function {
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

    it = attrs.find("activation_type");
    CH_STEST_RETURN2(it != attrs.end(), error::INVALID_ARGUMENT,
                     "attribute activate_type is needed in node definition");
    activate_type_ = it->second.string_val();

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
    Status status;
    CHECK_EQ(Inputs().size(), 1UL);
    CHECK_EQ(Outputs().size(), 1UL);
    if (!Inputs()[0]) {
      DLOG(WARNING) << "input nullptr, stop service";
      return status;
    }
    // no bias, just weight in the first version.
    CHECK_EQ(params_->size(), 1UL);

    auto &input = *Inputs()[0]->ArgField()->float_mat_val->MatPtr();
    auto &weight = *params_->at(0)->ArgField()->float_mat_val->MatPtr();
    auto &activation = *Output(0)->ArgField()->float_mat_val->MatPtr();

    activation = input * weight;

    return status;
  }

  Status BackwardCompute() override {

    Status status;

    CHECK_EQ(OutputGrads().size(), 1UL);
    CHECK_EQ(Inputs().size(), 1UL);
    CHECK_EQ(InputGrads().size(), 1UL);
    CHECK_EQ(params_->size(), 1UL);

    auto inputT = Input(0)->ArgField()->float_mat_val->MatPtr()->transpose();
    auto &outputGrad = *OutputGrad(0)->ArgField()->float_mat_val->MatPtr();
    auto &weight = *params_->at(0)->ArgField()->float_mat_val->MatPtr();

    // Calculate the weight's grad for current Node.
    if (!WeightGrads().empty()) {
      CHECK_EQ(WeightGrads().size(), 1UL);
      auto &weightGrad = *WeightGrad(0)->ArgField()->float_mat_val->MatPtr();
      weightGrad = inputT * outputGrad;
    }

    // Calculate the previous Node's grad.
    auto &preGrad = *InputGrad(0)->ArgField()->float_mat_val->MatPtr();
    auto weightT = weight.transpose();

    preGrad = outputGrad * weightT;
    return status;
  }

  virtual void CheckContext() override {}

private:
  int64_t dim_, batch_size_;
  std::string activate_type_;
  // Save forward computation's output.
  Argument *activation_;
};

} // namespace functions
} // chasky

#endif
