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

TEST(Argument,mat) {
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


}
}
