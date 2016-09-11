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

  virtual Status
  ForwardCompute(const std::vector<const Argument *> &args,
                 const std::vector<ArgumentPtr> *activation) override {
    Status status;
    for (size_t i = 0; i < args.size(); i++) {
      if (!args[i]) {
        DLOG(WARNING) << "input nullptr, stop service";
        return status;
      }
    }

    CHECK_EQ(args.size(), activation->size());
    CHECK_EQ(args.size(), 1UL);



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
  int64_t dim_, batch_size_;
  std::string activate_type_;
};

} // namespace functions
} // chasky

#endif
