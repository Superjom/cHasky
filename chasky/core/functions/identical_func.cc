#include "chasky/core/common/register.h"
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/functions/identical_func.h"

namespace chasky {
namespace functions {

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

REGISTER_FUNC(identical_func, CH_MAT_FLOAT,
              identical_func<math::CpuFloatMatrix>);
}
}
