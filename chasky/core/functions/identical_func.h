/*
 * IdenticalFunc.h
 *
 * Same to PaddlePaddle's IdentityProjection, outputs equals inputs.
 */
#ifndef CHASKY_CORE_FUNCTIONS_IDENTICAL_FUNC_H_
#define CHASKY_CORE_FUNCTIONS_IDENTICAL_FUNC_H_
#include "chasky/core/common/register.h"
#include "chasky/core/framework/function.h"
#include "chasky/core/functions/function_def.h"
#include "chasky/core/framework/attr_value_util.h"
#include "chasky/core/framework/function_def_builder.h"
#include "chasky/core/framework/argument_def_builder.h"

namespace chasky {
namespace functions {

REGISTER_FUNC_DEF(
    identical_func,
    FunctionDefBuilder()
        .Name("identical_func")
        // data type of input_list should be detemined in the runtime.
        .Input(ArgumentDefBuilder().Name("input").Type("float_mat").Finalize())
        // data type of output should be detemined in the runtime.
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

} // functions

} // namespace chasky
#endif
