#ifndef CHASKY_CORE_FUNCTIONS_ADDN_DEF_H_
#define CHASKY_CORE_FUNCTIONS_ADDN_DEF_H_
// AddN function's definition.
#include "chasky/core/common/register.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/attr_value_util.h"
#include "chasky/core/framework/function_def_builder.h"

namespace chasky {
namespace functions {

REGISTER_FUNC_DEF(
    AddN, FunctionDefBuilder()
               .Name("AddN")
               // data type of input_list should be detemined in the runtime.
               .Input(ArgumentDefBuilder().Name("input_list").Finalize())
               // data type of output should be detemined in the runtime.
               .Output(ArgumentDefBuilder().Name("output").Finalize())
               // the attributes should be specified in the runtime.
               .Attr(AttrDefBuilder().Name("dim").Type("int32").Finalize())
               // Use dtype to determine all inputs' datatype
               .Attr(AttrDefBuilder().Name("dtype").Type("dtype").Finalize())
               .Finalize());

} // namespace functions
} // namespace chasky
#endif
