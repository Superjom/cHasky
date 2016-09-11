#include <cstdlib>

#include "chasky/core/framework/argument_field.h"
namespace chasky {

void ArgumentField::CloneFrom(const ArgumentField &other) {
  uint64_val = other.uint64_val;
  int64_val = other.int64_val;
  float_mat_val = other.float_mat_val;
  float_mat_vals = other.float_mat_vals;
}

void ArgumentField::CopyFrom(const ArgumentField &other) {
  DLOG(INFO) << "ArgumentField::CopyFrom";
  if (other.uint64_val) {
    DLOG(INFO) << "to copy uint64_val ...";
    *uint64_val = *other.uint64_val;
  }
  if (other.int64_val) {
    DLOG(INFO) << "to copy int64_val ...";
    *int64_val = *other.int64_val;
  }
  if (other.float_mat_val) {
    DLOG(INFO) << "to copy float_mat_val ...";
    DLOG(INFO) << "other.mat " << other.float_mat_val.get();
    if (!float_mat_val) {
      float_mat_val =
          std::make_shared<math::CpuFloatMatrix>(*other.float_mat_val);
    } else {
      *float_mat_val = *other.float_mat_val;
    }
  }
  if (other.float_mat_vals) {
    DLOG(INFO) << "to copy float mat ...";
    *float_mat_vals = *other.float_mat_vals;
  }
}

bool ArgumentField::IsEmpty() const {
  return float_val == nullptr && uint64_val == nullptr &&
         float_mat_val == nullptr && float_mat_vals == nullptr &&
         int64_vals == nullptr && uint64_vals == nullptr &&
         float_vals == nullptr;
}

void ArgumentField::FromBuffer(const std::string &buffer, DataType dtype) {
  switch (dtype) {
  case DataType::CH_MAT_FLOAT: {
    math::CpuFloatMatrix *mat;
    get(&mat);
    mat->FromBuffer(buffer);
    // const auto& shape = mat->Shape();
    // size_t buffer_size = shape.first * shape.second * sizeof(float);
    // CHECK_EQ(buffer_size, buffer.size());

    // const char* bstart = buffer.c_str();
    // char* end;
    // for (size_t i = 0; i < shape.first; i++) {
    //   for (size_t j = 0; j < shape.second; j++) {
    //     *mat(i, j) = std::strtof(bstart, &end);
    //   }
    // }
  } break;
  default:
    UN_IMPLEMENTED
  }
}

} // namespace chasky
