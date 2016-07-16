#include <gtest/gtest.h>
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/argument.h"

using namespace chasky;

TEST(Argument, ref) {
  auto def = NewArgumentDefBuilder()
                 .Name("arg1")
                 .Type("float_vec")
                 .Shape(10, 1)
                 .SetIsRef(true)
                 .Finalize();
  Argument arg(&def);

  Argument arg1(arg);
}

TEST(Argument, matrix) {
  auto def = NewArgumentDefBuilder()
                 .Name("arg1")
                 .Type("float_mat")
                 .Shape(10, 20)
                 .SetIsRef(false)
                 .Finalize();
  Argument arg(&def);

  auto mat = arg.ArgField()->float_mat_val;
  LOG(INFO) << "mat:\t" << mat->DebugString();
}
