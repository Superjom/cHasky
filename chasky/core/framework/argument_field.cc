#include "chasky/core/framework/argument_field.h"
namespace chasky {

void ArgumentField::CloneFrom(const ArgumentField &other) {
  uint64_val = other.uint64_val;
  int64_val = other.int64_val;
  float_mat_val = other.float_mat_val;
  float_mat_vals = other.float_mat_vals;
}

void ArgumentField::CopyFrom(const ArgumentField &other) { UN_IMPLEMENTED }

bool ArgumentField::IsEmpty() const {
  return float_val == nullptr && uint64_val == nullptr &&
         float_mat_val == nullptr && float_mat_vals == nullptr &&
         int64_vals == nullptr && uint64_vals == nullptr &&
         float_vals == nullptr;
}

} // namespace chasky
