#ifndef CHASKY_CORE_FUNCTIONS_FULLY_CONNECTED_FUNC_DEF_H_
#define CHASKY_CORE_FUNCTIONS_FULLY_CONNECTED_FUNC_DEF_H_

#include "chasky/core/common/register.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/attr_value_util.h"
#include "chasky/core/framework/function_def_builder.h"

namespace chasky {
namespace functions {

REGISTER_FUNC_DEF(
    FullyConnectedFunc,
    FunctionDefBuilder()
        .Name("FullyConnectedFunc")
        .Input(ArgumentDefBuilder().Name("input").Type("float_mat").Finalize())
        .Output(
            ArgumentDefBuilder().Name("output").Type("float_mat").Finalize())
        .Attr(AttrDefBuilder()
                  .Name("dim")
                  .Type("int64")
                  .Doc("matrix dimention")
                  .Finalize())
        .Attr(AttrDefBuilder()
                  .Name("batch_size")
                  .Type("int64")
                  .Doc("size of the input batch")
                  .Finalize())
        .Finalize());

} // namespace functions
} // namespace chasky
#endif
