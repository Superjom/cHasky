#include <gtest/gtest.h>
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/framework/function.h"
#include "chasky/core/framework/function_def_builder.h"
#include "chasky/core/functions/AddN.h"
// TODO Add AddN FromDef test
namespace chasky {
namespace test {

class AddN_Test : public ::testing::Test {
protected:
  virtual void SetUp() {
    LOG(INFO) << FunctionDefLibrary::Instance().DebugString();
    ASSERT_TRUE(FunctionDefLibrary::Instance().LookUp("AddN", &def).ok());

    auto func_sign = Function::Signature("AddN", CH_MAT_FLOAT);
    auto status = FunctionLibrary::Instance().LookUp(func_sign, &creator);
    EXPECT_TRUE(status.ok());
    if (!status.ok()) {
      LOG(ERROR) << status.msg();
    }
  }

protected:
  FunctionDef *def;
  FunctionLibrary::FunctionCreatorType *creator;
};

TEST_F(AddN_Test, init) {}

TEST_F(AddN_Test, FromDef) {
  // Fill the missing attributes
  NodeDef node_def;
  (*node_def.mutable_attr())["dim"] =
      AttrValueBuilder().Value((int64_t)200).Finalize();
  (*node_def.mutable_attr())["dtype"] =
      AttrValueBuilder().Value(CH_MAT_FLOAT_LIST).Finalize();

  auto function = (*creator)();
  ASSERT_TRUE(function->FromDef(*def, node_def.attr()).ok());

  auto addn =
      dynamic_cast<functions::AddN<math::CpuFloatMatrix> *>(function.get());

  EXPECT_EQ(addn->Dim(), 200);
  EXPECT_EQ(addn->Dtype(), CH_MAT_FLOAT_LIST);
}

// The arguments' preparation is done by Nodes(the upper level), so in function
// level, much work should done manully.
TEST_F(AddN_Test, ForwardCompute) {
  // prepare inputs, just one argument list field.
  DLOG(INFO) << "preparing arguments";
  const size_t width = 20;
  const size_t height = 40;

  // input argument's definition
  auto arg_def = ArgumentDefBuilder()
                     .Name("input1")
                     .Type("float_mat_list")
                     .SetIsRef(false)
                     .Finalize();
  // activation's definition
  auto arg_def_act = ArgumentDefBuilder()
                         .Name("input1")
                         .Type("float_mat")
                         .Shape(width, height)
                         .SetIsRef(false)
                         .Finalize();

  Argument arg(&arg_def);
  Argument act(&arg_def_act);

  std::vector<std::shared_ptr<math::CpuFloatMatrix>> mats;
  auto shape = std::make_pair(width, height);
  for (size_t i = 0; i < width; i++) {
    mats.emplace_back(std::make_shared<math::CpuFloatMatrix>(shape, true));
  }

  *(arg.ArgField()->float_mat_vals) = mats;

  std::vector<const Argument *> inputs = {&arg};
  std::vector<Argument *> outputs = {&act};

  DLOG(INFO) << "begin forward";
  // create addn
  NodeDef node_def;
  (*node_def.mutable_attr())["dim"] =
      AttrValueBuilder().Value((int64_t)width).Finalize();
  (*node_def.mutable_attr())["dtype"] =
      AttrValueBuilder().Value(CH_MAT_FLOAT_LIST).Finalize();

  auto function = (*creator)();
  ASSERT_TRUE(function->FromDef(*def, node_def.attr()).ok());

  auto status = function->ForwardCompute(inputs, &outputs, *def);
  ASSERT_TRUE(status.ok());
}

TEST_F(AddN_Test, BackwardCompute) {
  DLOG(INFO) << "preparing arguments";
  const size_t width = 20;
  const size_t height = 40;

  // input argument's definition
  auto arg_def = ArgumentDefBuilder()
                     .Name("input1")
                     .Type("float_mat_list")
                     .SetIsRef(false)
                     .Finalize();
  // activation's definition
  auto arg_def_act = ArgumentDefBuilder()
                         .Name("input1")
                         .Type("float_mat")
                         .Shape(width, height)
                         .SetIsRef(false)
                         .Finalize();

  Argument x_arg(&arg_def), grad_arg(&arg_def_act), previous_grad_arg(&arg_def);

  auto shape = std::make_pair(width, height);
  for (size_t i = 0; i < 10 /*num_input_node*/; i++) {
    x_arg.ArgField()->float_mat_vals->emplace_back(
        std::make_shared<math::CpuFloatMatrix>(shape));

    previous_grad_arg.ArgField()->float_mat_vals->emplace_back(
        std::make_shared<math::CpuFloatMatrix>(shape, /*random*/ false));
  }
  std::vector<const Argument *> x({&x_arg});
  std::vector<Argument *> previous_grad({&previous_grad_arg});

  // create addn
  NodeDef node_def;
  // fill all the missing attributes
  (*node_def.mutable_attr())["dim"] =
      AttrValueBuilder().Value((int64_t)width).Finalize();
  (*node_def.mutable_attr())["dtype"] =
      AttrValueBuilder().Value(CH_MAT_FLOAT_LIST).Finalize();

  auto function = (*creator)();
  ASSERT_TRUE(function->FromDef(*def, node_def.attr()).ok());

  // begin computation
  EXPECT_TRUE(function->BackwardCompute(x, grad_arg, &previous_grad).ok());
}

} // namespace test
} // namespace chasky
