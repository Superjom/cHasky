#include "chasky/core/framework/argument_field.h"
namespace chasky {

void ArgumentField::CopyFrom(const ArgumentField &other, bool is_ref) {
  if (is_ref) {
    uint64_val = other.uint64_val;
    int64_val = other.int64_val;
    float_mat_val = other.float_mat_val;
  } else { // copy mode
    UN_IMPLEMENTED
  }
}

void ArgumentField::RealCopyFrom(const ArgumentField &other) {
  // float_vec_val =
  // std::make_shared<math::CpuFloatVector>(*other.float_vec_val);
}

bool ArgumentField::IsEmpty() const {
  return float_val == nullptr && uint64_val == nullptr &&
         float_mat_val == nullptr && float_mat_vals == nullptr &&
         int64_vals == nullptr && uint64_vals == nullptr &&
         float_vals == nullptr;
}

} // namespace chasky
