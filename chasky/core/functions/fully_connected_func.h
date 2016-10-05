/*
 * FullyConnectedFunc.h
 *
 * Same to PaddlePaddle's fc layer, multi-layer neural networks.
 */
#ifndef CHASKY_CORE_FUNCTIONS_FULLY_CONNECTED_FUNC_H_
#define CHASKY_CORE_FUNCTIONS_FULLY_CONNECTED_FUNC_H_
#include "chasky/core/common/register.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/attr_value_util.h"
#include "chasky/core/framework/function.h"
#include "chasky/core/functions/function_def.h"
#include "chasky/core/framework/function_def_builder.h"

namespace chasky {
namespace functions {

// fully_connected_func is a fully connected hidden layer in neural networks.
// The mathematical formula is: fully_connected_func(x) ~ Wx, W is a matrix
// which is model parameter.
// It has two attributes:
//   1. dim: dimention of input vector
//   2. batch_size: batch size of the input records
REGISTER_FUNC_DEF(
    fully_connected_func,
    FunctionDefBuilder()
        .Name("fully_connected_func")
        .Input(ArgumentDefBuilder().Name("input").Type("float_mat").Finalize())
        .Output(
            ArgumentDefBuilder().Name("output").Type("float_mat").Finalize())
        .Attr(AttrDefBuilder()
                  .Name("input_dim")
                  .Type("int64")
                  .Doc("input's dimention")
                  .Finalize())
        .Attr(AttrDefBuilder()
                  .Name("batch_size")
                  .Type("int64")
                  .Doc("size of the input batch")
                  .Finalize())
        .Attr(AttrDefBuilder()
                  .Name("output_dim")
                  .Type("int64")
                  .Doc("matrix dimention")
                  .Finalize())
        .Finalize());

} // namespace functions
} // chasky

#endif
