#ifndef CHASKY_CORE_COMMON_EIGEN_MATRIX_H_
#define CHASKY_CORE_COMMON_EIGEN_MATRIX_H_
#include <iostream>
#include <sstream>
#include <glog/logging.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include "chasky/core/common/matrix.h"

namespace chasky {

namespace math {

// Eigen Cpu Matrix
template <class Type> class EigenMatrix : public BaseMatrix<Type, CPU> {
public:
  using base_matrix_t = BaseMatrix<Type, CPU>;
  using eigen_matrix_t =
      Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
  using self_type = EigenMatrix<Type>;
  using value_type = Type;
  typedef std::pair<size_t, size_t> shape_t;

  EigenMatrix() : mat_(nullptr) {}

  EigenMatrix(const std::shared_ptr<eigen_matrix_t> mat) : mat_(mat) {}

  EigenMatrix(const self_type &other) : mat_(other.MatPtr()) {}

  EigenMatrix(const shape_t &shape, bool random_init = true)
      : mat_(std::make_shared<eigen_matrix_t>(shape.first, shape.second)) {
    if (random_init) {
      *mat_ = eigen_matrix_t::Random(shape.first, shape.second);
    } else {
      *mat_ = eigen_matrix_t::Zero(shape.first, shape.second);
    }
  }

  virtual base_matrix_t &operator=(const base_matrix_t &other) override {
    auto _other = dynamic_cast<const self_type &>(other);
    mat_ = _other.MatPtr();
    return *this;
  }
  // TODO add real copy from method to copy memory

  virtual void MultWith(const base_matrix_t &other, float ratio1 = 1.,
                        float ratio2 = 1.) override {

    // TODO add shape check here.
    auto other_mat = reinterpret_cast<const eigen_matrix_t *>(other.RawMat());
    *mat_ = (*mat_ * ratio1) * (*other_mat * ratio2);
  }

  virtual void AddWith(const base_matrix_t &other, float ratio1 = 1.,
                       float ratio2 = 1.) override {
    auto other_mat = reinterpret_cast<const eigen_matrix_t *>(other.RawMat());
    // check shape
    CHECK_EQ(mat_->rows(), other.Shape().first);
    CHECK_EQ(mat_->cols(), other.Shape().second);
    *mat_ = *mat_ * ratio1 + *other_mat * ratio2;
  }

  virtual const Type *Data() const override { return mat_->data(); }

  virtual const void *RawMat() const override { return (void *)mat_.get(); }

  virtual shape_t Shape() const override {
    return std::make_pair(mat_->rows(), mat_->cols());
  }

  virtual std::string DebugString() const override {
    std::stringstream ss;
    ss << *mat_;
    return ss.str();
  }

  virtual Type Get(size_t i, size_t j) const override { return (*mat_)(i, j); }

  std::shared_ptr<eigen_matrix_t> MatPtr() const { return mat_; }

  const eigen_matrix_t &Mat() const { return *mat_; }

private:
  std::shared_ptr<eigen_matrix_t> mat_;
};

using CpuFloatMatrix = EigenMatrix<float>;
using DoubleFloatMatrix = EigenMatrix<double>;
using CpuInt32Matrix = EigenMatrix<int32_t>;
using CpuInt64Matrix = EigenMatrix<int64_t>;

} // namespace math

} //  namespace chasky
#endif
