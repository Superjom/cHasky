#include "chasky/core/common/register.h"
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/functions/IdenticalFunc.h"

namespace chasky {
namespace functions {

REGISTER_FUNC(IdenticalFunc, CH_MAT_FLOAT, IdenticalFunc<math::CpuFloatMatrix>);
}
}
