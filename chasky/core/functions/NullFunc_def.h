#ifndef CHASKY_CORE_FUNCTIONS_NULL_FUNC_DEF_H_
#define CHASKY_CORE_FUNCTIONS_NULL_FUNC_DEF_H_

#include "chasky/core/common/register.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/attr_value_util.h"
#include "chasky/core/framework/function_def_builder.h"

namespace chasky {
namespace functions {

REGISTER_FUNC_DEF(
    NullFunc,
    FunctionDefBuilder()
        .Name("NullFunc")
        // data type of input_list should be detemined in the runtime.
        .Input(ArgumentDefBuilder().Name("input").Type("float_mat").Finalize())
        // data type of output should be detemined in the runtime.
        .Output(
            ArgumentDefBuilder().Name("output").Type("float_mat").Finalize())
        .Finalize());
} // namespace functions
} // namespace chasky
#endif
