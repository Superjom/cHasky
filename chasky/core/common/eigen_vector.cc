#include <sstream>
#include "chasky/core/common/eigen_vector.h"

using namespace chasky;
using namespace chasky::math;

template <class Type> std::string EigenVector<Type>::DebugString() const {
  std::stringstream ss;
  ss << vec_;
  return ss.str();
}

template <class Type> size_t EigenVector<Type>::Size() const {
  return vec_.size();
}

/*
template <class Type>
Type EigenVector<Type>::Get(size_t i) const {
  return vec_[i];
}
*/

// explicitly instantiate all the template instances to keep definitions
// and implementations seperate
template class chasky::math::EigenVector<int32_t>;
template class chasky::math::EigenVector<int64_t>;
template class chasky::math::EigenVector<double>;
template class chasky::math::EigenVector<float>;
