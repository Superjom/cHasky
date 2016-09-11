#include "chasky/core/common/register.h"
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/functions/NullFunc.h"

namespace chasky {
namespace functions {

REGISTER_FUNC(NullFunc, CH_MAT_FLOAT, NullFunc<math::CpuFloatMatrix>);
}
}
