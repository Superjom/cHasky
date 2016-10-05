#include "chasky/core/common/register.h"
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/functions/rmse_loss_func.h"

namespace chasky {
namespace functions {

template <typename T> class rmse_loss_func : public Function {
public:
  virtual Status FromDef(FunctionDef &func_def,
                         const Function::extra_attr_t &attrs) override {
    Status status;
    auto it = attrs.find("dim");
    CH_STEST_RETURN2(it != attrs.end(), error::INVALID_ARGUMENT,
                     "attribute dim is needed in node definition");
    dim_ = it->second.int64_val();

    it = attrs.find("batch_size");
    CH_STEST_RETURN2(it != attrs.end(), error::INVALID_ARGUMENT,
                     "attribute batch_size is needed in node definition");
    batch_size_ = it->second.int64_val();
    DLOG(INFO) << "set attr batch_size to " << batch_size_;

    CHECK_GT(dim_, 0UL);
    CHECK_GT(batch_size_, 0UL);

    def_ = &func_def;
    for (auto &def : *def_->mutable_outputs()) {
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
    CHECK_EQ(Inputs().size(), 2UL);
    CHECK_EQ(Outputs().size(), 1UL);
    CHECK(Input(0));
    CHECK(Input(1));
    CHECK(Output(0));

    auto &target = *Input(0)->ArgField()->float_mat_val->MatPtr();
    auto &label = *Input(1)->ArgField()->float_mat_val->MatPtr();
    auto &loss = *Output(0)->ArgField()->float_mat_val->MatPtr();
    loss = (target - label);
    loss = loss.cwiseProduct(loss);
    return status;
  }

  virtual Status BackwardCompute() override {
    Status status;
    CHECK_EQ(InputGrads().size(), 1UL);
    CHECK_EQ(OutputGrads().size(), 1UL);

    auto &preGrad = *InputGrad(0)->ArgField()->float_mat_val->MatPtr();
    auto &grad = *InputGrad(0)->ArgField()->float_mat_val->MatPtr();

    return status;
  }

  virtual void CheckContext() override {}

private:
  int64_t dim_, batch_size_;
};

REGISTER_FUNC(rmse_loss_func, CH_MAT_FLOAT,
              rmse_loss_func<math::CpuFloatMatrix>);
}

}
