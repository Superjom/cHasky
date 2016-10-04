#include "chasky/core/common/register.h"
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/functions/identical_func.h"

namespace chasky {
namespace functions {

REGISTER_FUNC(identical_func, CH_MAT_FLOAT, identical_func<math::CpuFloatMatrix>);

}
}
