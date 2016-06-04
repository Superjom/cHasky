#ifndef CHASKY_CORE_COMMON_EIGEN_VECTOR_H_
#define CHASKY_CORE_COMMON_EIGEN_VECTOR_H_
#include <glog/logging.h>
#include "chasky/core/common/vector.h"

namespace chasky {

namespace math {

template <class Type> class EigenVector : public CpuBaseVector<Type> {
public:
  using eigen_vector_type = Eigen::Matrix<Type, Eigen::Dynamic, 1>;
  using base_vector_type = CpuBaseVector<Type>;
  using self_type = EigenVector<Type>;
  using value_type = Type;

  EigenVector(size_t size) : vec_(size) {}

  EigenVector(const std::vector<value_type> &raw_data) 
      : vec_(raw_data.size()) {
    for (size_t i = 0; i < raw_data.size(); i++) {
      vec_[i] = raw_data[i];
    }
  }

  EigenVector(const value_type *raw_data, size_t size): vec_(size) {
    for (size_t i = 0; i < size; i++) {
      vec_[i] = raw_data[i];
    }
  }

  EigenVector(const EigenVector &other) :
    vec_(*reinterpret_cast<const eigen_vector_type*>(other.Data())) {}

  EigenVector(const eigen_vector_type &other) : vec_(other) {}

  virtual base_vector_type& operator=(const base_vector_type &other) override {
    vec_ = *reinterpret_cast<const eigen_vector_type*>(other.Data());
    return *this;
  }

  virtual const void* Data() const override {
    return reinterpret_cast<const void*>(&vec_); }

  virtual value_type DotWith(const base_vector_type &other) override {
    CHECK_EQ(Size(), other.Size());
    return vec_.dot(*reinterpret_cast<const eigen_vector_type*>(other.Data()));
  }

  virtual base_vector_type Sub(size_t start_index, size_t end_size = 0) override
  {
    CHECK_GE(start_index, 0);
    CHECK_LE(end_size, vec_.size());
    return EigenVector(vec_.block(start_index, 0, end_size, 1));
  }

  virtual base_vector_type& SetZero() override {
    //vec_.Zero();
    for (size_t i = 0; i < vec_.size(); i++) {
      vec_[i] = 0;
    }
    return *this;
  }

  // TODO support random by gaussian distribution
  virtual base_vector_type& Randomize() override {
    vec_ = eigen_vector_type::Random(Size(), 1);
    return *this;
  }

  virtual std::string DebugString() const override;

  virtual size_t Size() const override;

  virtual Type Get(size_t i) const override {
    return vec_[i];
  }

private:
  eigen_vector_type vec_;
}; // class EigenVector

} // namespace math
} // namespace chasky

#endif
