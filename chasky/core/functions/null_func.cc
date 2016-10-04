#include "chasky/core/common/register.h"
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/functions/null_func.h"

namespace chasky {
namespace functions {

REGISTER_FUNC(null_func, CH_MAT_FLOAT, null_func<math::CpuFloatMatrix>);

}
}
