#include <vector>
#include <gtest/gtest.h>
#include "chasky/core/common/eigen_vector.h"
using namespace std;
using namespace chasky::math;

TEST(EigenVector, init) {
  // init from std::vector
  std::vector<int> vec = {1, 2, 3, 4, 5};
  EigenVector<int> eigen_vec(vec);

  std::vector<int> vec2 = {3, 4, 5, 6, 7};
  EigenVector<int> eigen_vec2(vec2);

  cout << eigen_vec.DebugString() << endl;

  auto dot_res = eigen_vec.DotWith(eigen_vec2);
  ASSERT_EQ(dot_res, 85);
}

TEST(EigenVector, SetZero) {
  EigenVector<int> vec(10);
  vec.SetZero();
  for (int i = 0; i < 10; i++) {
    ASSERT_EQ(vec.Get(i), 0);
  }
}

TEST(EigenVector, Randomize) {
  EigenVector<float> vec(10);
  vec.Randomize();
  std::cout << vec.DebugString() << std::endl;
}
