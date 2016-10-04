#include "chasky/core/functions/fully_connected_func.h"

namespace chasky {
namespace functions {

REGISTER_FUNC(fully_connected_func, CH_MAT_FLOAT,
              fully_connected_func<math::CpuFloatMatrix>);

} // namespace functions
} // namespace chasky
