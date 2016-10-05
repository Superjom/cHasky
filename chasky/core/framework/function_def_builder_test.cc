#include <gtest/gtest.h>
#include "chasky/core/framework/attr_value_util.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/function_def_builder.h"
using namespace std;
using namespace chasky;

TEST(FunctionDefBuilder, init) {
  FunctionDefBuilder builder;
  builder.Name("layer1")
      .Attr(NewAttrDefBuilder()
                .Name("size")
                .Type("int64")
                .Doc("size of this layer")
                .Finalize())

      .Input(NewArgumentDefBuilder()
                 .Name("input1")
                 .Type("float_mat")
                 .Doc("a float vector")
                 .Finalize())

      .Output(NewArgumentDefBuilder()
                  .Name("output1")
                  .Type("float_mat")
                  .Doc("a float vector")
                  .Finalize());

  LOG(INFO) << "operator def:\n" << builder.DebugString();
}
