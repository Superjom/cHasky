#include <gtest/gtest.h>
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/argument.h"

namespace chasky {

namespace test {

TEST(Argument, ref) {
  auto def = ArgumentDefBuilder()
                 .Name("arg1")
                 .Type("float_mat")
                 .Shape(10, 30)
                 .SetIsRef(true)
                 .Finalize();
  Argument arg(&def);
  Argument arg1(arg);

  EXPECT_EQ(arg.ArgField(), arg1.ArgField());
}

// TODO add copy test

TEST(Argument, mat) {
  auto def = ArgumentDefBuilder()
                 .Name("arg1")
                 .Type("float_mat")
                 .Shape(10, 30)
                 .SetIsRef(true)
                 .Finalize();
  Argument arg1(&def);
  ASSERT_TRUE(arg1.ArgField()->float_mat_val != nullptr);

  auto shape = arg1.ArgField()->float_mat_val->Shape();
  EXPECT_EQ(shape.first, 10);
  EXPECT_EQ(shape.second, 30);
}

class ArgumentTest : public ::testing::Test {
public:
  void SetUp() {
    arg_def_ = ArgumentDefBuilder()
                   .Name("arg1")
                   .Type("float_mat")
                   .Shape(10, 30)
                   .Finalize();
    arg_ = std::make_shared<Argument>(&arg_def_);
  }

protected:
  ArgumentDef arg_def_;
  std::shared_ptr<Argument> arg_;
};

TEST_F(ArgumentTest, init) {}
TEST_F(ArgumentTest, shape) {
  math::CpuFloatMatrix *mat;
  arg_->ArgField()->get(&mat);
  ASSERT_EQ(mat->Shape().first, 10);
  ASSERT_EQ(mat->Shape().second, 30);
}

TEST_F(ArgumentTest, Serialize) {
  ArgumentDef buffer;
  arg_->Serialize(&buffer);
  ASSERT_FALSE(buffer.content().empty());
}

TEST_F(ArgumentTest, DeSerialize) {
  Argument new_arg(&arg_def_);
  ArgumentDef buffer(arg_def_);
  arg_->Serialize(&buffer);

  size_t buffer_size =
      arg_def_.shape().width() * arg_def_.shape().height() * sizeof(float);

  ASSERT_EQ(buffer.content().size(), buffer_size);

  new_arg.DeSerialize(buffer);

  math::CpuFloatMatrix *src_mat, *trg_mat;
  arg_->ArgField()->get(&src_mat);
  arg_->ArgField()->get(&trg_mat);

  DLOG(INFO) << "to check equality between src_mat and trg_mat";
  for (size_t i = 0; i < src_mat->Shape().first; i++) {
    for (size_t j = 0; j < src_mat->Shape().second; j++) {
      ASSERT_EQ(src_mat->Get(i, j), trg_mat->Get(i, j));
    }
  }

  LOG(INFO) << "source matrix: " << src_mat->DebugString();
  LOG(INFO) << "target matrix: " << trg_mat->DebugString();
}
}
}
