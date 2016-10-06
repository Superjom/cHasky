#ifndef CHASKY_CORE_FUNCTIONS_RMSE_LOSS_FUNC_H_
#define CHASKY_CORE_FUNCTIONS_RMSE_LOSS_FUNC_H_

#include "chasky/core/common/register.h"
#include "chasky/core/framework/function.h"
#include "chasky/core/functions/function_def.h"
#include "chasky/core/framework/attr_value_util.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/function_def_builder.h"

namespace chasky {
namespace functions {

REGISTER_FUNC_DEF(
    rmse_loss_func,
    FunctionDefBuilder()
        .Name("rmse_loss_func")
        .Input(ArgumentDefBuilder()
                   .Name("prediction")
                   .Type("float_mat")
                   .Finalize())
        .Input(ArgumentDefBuilder().Name("target").Type("float_mat").Finalize())
        .Output(ArgumentDefBuilder().Name("loss").Type("float_mat").Finalize())
        .Attr(AttrDefBuilder()
                  .Name("dim")
                  .Type("int64")
                  .Doc("dimention, both the input and label "
                       "should have the same dimention")
                  .Finalize())
        .Attr(AttrDefBuilder()
                  .Name("batch_size")
                  .Type("int64")
                  .Doc("size of the input batch")
                  .Finalize())
        .FuncType(FunctionDef_FuncType_LOSS)
        .Finalize());

} // namespace functions
} // namespace chasky
#endif
