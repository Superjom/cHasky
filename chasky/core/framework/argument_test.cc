#include <gtest/gtest.h>
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/argument.h"

using namespace chasky;

TEST(Argument, ref_count) {
  auto def = NewArgumentDefBuilder().Name("arg1")
             .Type("float_vec")
             .Shape(10, 1)
             .SetIsRef(true)
             .Finalize();
  Argument arg(&def);

  Argument arg1(arg);
}
