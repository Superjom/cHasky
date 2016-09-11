#include <glog/logging.h>
#include <gtest/gtest.h>
#include "chasky/core/common/eigen_matrix.h"

namespace chasky {

namespace test {
using namespace math;

TEST(CpuFloatMatrix, random_init) {
  auto shape = std::make_pair(20, 10);
  CpuFloatMatrix mat(shape);
  LOG(INFO) << mat.DebugString();
  int zero_count = 0;
  for (size_t i = 0; i < shape.first * shape.second; i++) {
    if (mat.Data()[i] == 0.)
      zero_count++;
  }
  ASSERT_LE(zero_count, shape.first * shape.second / 2);
}

TEST(CpuFloatMatrix, zero_init) {
  auto shape = std::make_pair(20, 10);
  CpuFloatMatrix mat(shape, false);
  LOG(INFO) << "zero:" << mat.DebugString();
  for (size_t i = 0; i < shape.first * shape.second; i++) {
    ASSERT_EQ(mat.Data()[i], 0.);
  }
}

TEST(CpuFloatMatrix, mult) {
  auto shape = std::make_pair(3, 5);
  auto shape2 = std::make_pair(5, 3);
  CpuFloatMatrix mat1(shape, true);
  CpuFloatMatrix mat2(shape2, true);

  LOG(INFO) << "mat1\t" << mat1.DebugString();
  LOG(INFO) << "mat2\t" << mat2.DebugString();

  mat1.MultWith(mat2);

  LOG(INFO) << mat1.DebugString();
}

TEST(CpuFloatMatrix, add) {
  auto shape = std::make_pair(3, 6);
  CpuFloatMatrix mat1(shape, true);
  CpuFloatMatrix mat2(shape, true);

  LOG(INFO) << "mat1\t" << mat1.DebugString();
  LOG(INFO) << "mat2\t" << mat2.DebugString();

  EXPECT_TRUE(mat1.Get(0, 3) != mat2.Get(0, 3));
  LOG(INFO) << "add\t" << mat1.DebugString();

  mat1.AddWith(mat2);
}

TEST(CpuFloatMatrix, reshape) {
  auto shape = std::make_pair(3, 4);
  CpuFloatMatrix mat1(shape, true);

  mat1.Reshape(4, 3);

  auto shape2 = mat1.Shape();
  EXPECT_EQ(shape2.first, 4);
  EXPECT_EQ(shape2.second, 3);
}

TEST(CpuFloatMatrix, SetZero) {
  auto shape = std::make_pair(3, 4);
  CpuFloatMatrix mat1(shape, true);

  EXPECT_TRUE(mat1.Get(0, 1) != 0.);
  mat1.SetZero();
  EXPECT_TRUE(mat1.Get(0, 1) == 0.);
}

class CpuFloatMatrixTest : public ::testing::Test {
public:
  virtual void SetUp() {
    auto shape = std::make_pair(20, 40);
    CpuFloatMatrix mat1(shape);

    mat.CopyFrom(mat1);
  }

protected:
  CpuFloatMatrix mat;
};

TEST_F(CpuFloatMatrixTest, CloneFrom) {
  auto shape = std::make_pair(3, 4);
  CpuFloatMatrix mat1(shape);
  auto mat_ptr = mat.MatPtr();

  mat.CloneFrom(mat1);
  EXPECT_EQ(mat1.MatPtr(), mat.MatPtr());
  EXPECT_TRUE(mat1.MatPtr() != mat_ptr);
}

TEST_F(CpuFloatMatrixTest, CopyFrom) {
  auto shape = std::make_pair(30, 90);
  CpuFloatMatrix mat1(shape, true);
  mat.CopyFrom(mat1);
  EXPECT_TRUE(mat.MatPtr() != mat1.MatPtr());

  EXPECT_EQ(mat.Get(0, 4), mat1.Get(0, 4));
}

TEST_F(CpuFloatMatrixTest, Serialize_DeSerialize) {
  // serialize
  auto ss = mat.Serialize();
  auto shape = mat.Shape();
  auto size = shape.first * shape.second;
  ASSERT_EQ(size * sizeof(float), ss.size());

  CpuFloatMatrix tmp;
  tmp.CopyFrom(mat);

  // de-serialize
  mat.SetZero();
  ASSERT_TRUE(mat.DeSerialize(ss).ok());

  LOG(INFO) << "shape:\t" << shape.first << "\t" << shape.second;

  for (size_t w = 0; w < shape.first; w++) {
    for (size_t h = 0; h < shape.second; h++) {
      auto a = mat.Get(w, h);
      auto b = tmp.Get(w, h);
      EXPECT_EQ(a, b);
    }
  }
}

TEST_F(CpuFloatMatrixTest, FromBuffer) {
  auto shape = std::make_pair(20, 40);
  CpuFloatMatrix mat(shape, true);
  std::string buffer;
  mat.ToBuffer(buffer);

  CpuFloatMatrix mat1(shape, true);
  mat1.FromBuffer(buffer);

  for (size_t i = 0; i < 20; i++) {
    for (size_t j = 0; j < 40; j++) {
      EXPECT_EQ(mat1.Get(i, j), mat.Get(i, j));
    }
  }
}

} // namespace test
} // namespace chasky
