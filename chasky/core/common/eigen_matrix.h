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

template <class Type>
using EigenMatrixType =
    Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

// Eigen Cpu Matrix
template <class Type>
class EigenMatrix : public BaseMatrix<Type, EigenMatrixType<float>, CPU> {
public:
  using eigen_matrix_t = EigenMatrixType<Type>;
  using base_matrix_t = BaseMatrix<Type, EigenMatrixType<Type>, CPU>;
  using self_type = EigenMatrix<Type>;
  using value_type = Type;
  using shape_t = std::pair<size_t, size_t>;

  EigenMatrix() : mat_(nullptr) {}

  EigenMatrix(std::shared_ptr<eigen_matrix_t> mat) : mat_(mat) {}

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
    auto other_mat = other.MatPtr();
    *mat_ = (*mat_ * ratio1) * (*other_mat * ratio2);
  }

  virtual void AddWith(const base_matrix_t &other, float ratio1 = 1.,
                       float ratio2 = 1.) override {
    auto other_mat = other.MatPtr();
    // check shape
    CHECK_EQ(mat_->rows(), other.Shape().first);
    CHECK_EQ(mat_->cols(), other.Shape().second);
    *mat_ = *mat_ * ratio1 + *other_mat * ratio2;
  }

  virtual void Reshape(size_t width, size_t height) override {
    mat_->resize(width, height);
  }

  virtual void SetZero() override {
    const auto &shape = Shape();
    memset(mat_->data(), 0, shape.first * shape.second);
  }

  virtual void CloneFrom(base_matrix_t &other) override {
    mat_ = other.MatPtr();
  }

  virtual void CopyFrom(base_matrix_t &other) override {
    mat_ = std::make_shared<eigen_matrix_t>(*other.MatPtr());
  }

  virtual const Type *Data() const override { return mat_->data(); }

  virtual const void *RawMat() const override { return (void *)mat_.get(); }

  virtual shape_t Shape() const override {
    return std::make_pair(mat_->rows(), mat_->cols());
  }

  // Return a human-readable string.
  virtual std::string DebugString() const override;

  // Get the element on i-th row and j-th col
  virtual Type Get(size_t i, size_t j) const override { return (*mat_)(i, j); }

  virtual std::shared_ptr<eigen_matrix_t> MatPtr() const override {
    return mat_;
  }

  const eigen_matrix_t &Mat() const { return *mat_; }

  // Serialize the matrix's data to a binary string.
  virtual std::string Serialize() const override;

  // De-serialize a binary string and fill the matrix.
  // NOTE The matrix should be created and initialized first.
  virtual Status DeSerialize(const std::string &ss) override;

private:
  std::shared_ptr<eigen_matrix_t> mat_;
};

using CpuFloatMatrix = EigenMatrix<float>;
using DoubleFloatMatrix = EigenMatrix<double>;
using CpuInt32Matrix = EigenMatrix<int32_t>;
using CpuInt64Matrix = EigenMatrix<int64_t>;

template <typename Type>
using EigenMatrixPtr = std::shared_ptr<EigenMatrix<Type>>;

template <typename Type> std::string EigenMatrix<Type>::DebugString() const {
  std::stringstream ss;
  ss << *mat_;
  return ss.str();
}

template <typename Type> std::string EigenMatrix<Type>::Serialize() const {
  std::string ss;
  const auto size = Shape().first * Shape().second;
  ss.resize(sizeof(value_type) * size);
  memcpy(&ss[0], (unsigned char *)Data(), size * sizeof(value_type));
  return ss;
}

template <typename Type>
Status EigenMatrix<Type>::DeSerialize(const std::string &ss) {
  const auto shape = Shape();
  auto size = Shape().first * Shape().second;

  Status status;
  CH_TEST_OR_UPDATE_STATUS(mat_ != nullptr, error::NOT_INITED,
                           "matrix is not initialized.");
  CH_TEST_OR_UPDATE_STATUS(
      ss.size() == size * sizeof(value_type), error::OUT_OF_RANGE,
      strings::Printf("matrix de-serialize error;\nmatrix's shape:(%lu,%lu), \
                        serialized string's size is %lu",
                      shape.first, shape.second, ss.size()));
  memcpy(mat_->data(), ss.data(), size * sizeof(value_type));
  return status;
}

} // namespace math

} //  namespace chasky
#endif
