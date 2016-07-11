#include <glog/logging.h>
#include <gtest/gtest.h>
#include "chasky/core/common/eigen_matrix.h"

namespace chasky {

namespace test {
using namespace math;

TEST(EigenMatrix, random_init) {
  auto shape = std::make_pair(20, 10);
  float_mat_t mat(shape);
  LOG(INFO) << mat.DebugString();
  int zero_count = 0;
  for (size_t i = 0; i < shape.first * shape.second; i++) {
    if (mat.Data()[i] == 0.)
      zero_count++;
  }
  ASSERT_LE(zero_count, shape.first * shape.second / 2);
}

TEST(EigenMatrix, zero_init) {
  auto shape = std::make_pair(20, 10);
  float_mat_t mat(shape, false);
  LOG(INFO) << "zero:" << mat.DebugString();
  for (size_t i = 0; i < shape.first * shape.second; i++) {
    ASSERT_EQ(mat.Data()[i], 0.);
  }
}

TEST(EigenMatrix, mult) {
  auto shape = std::make_pair(3, 5);
  auto shape2 = std::make_pair(5, 3);
  float_mat_t mat1(shape, true);
  float_mat_t mat2(shape2, true);

  mat1.MultWith(mat2);

  LOG(INFO) << mat1.DebugString();
}

} // namespace test
} // namespace chasky
