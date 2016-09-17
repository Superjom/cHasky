#include "chasky/core/functions/FullyConnectedFunc.h"

namespace chasky {
namespace functions {

REGISTER_FUNC(FullyConnectedFunc, CH_MAT_FLOAT,
              FullyConnectedFunc<math::CpuFloatMatrix>);

} // namespace functions
} // namespace chasky
