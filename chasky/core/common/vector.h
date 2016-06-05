#ifndef CHASKY_CORE_COMMON_VECTOR_H_
#define CHASKY_CORE_COMMON_VECTOR_H_
#include <memory>
#include <vector>
#include "eigen3/Eigen/Core"
#include "chasky/core/common/status.h"
#include "chasky/core/framework/kernel.h"
namespace chasky {

template<class Type, KernelType K>
class BaseVector {
public:
  typedef Type value_type;

  //BaseVector(const std::vector<value_type>& raw_data) {}
  //BaseVector(const value_type* raw_data, size_t size) {}
  //BaseVector(const BaseVector& other) {}
  virtual BaseVector& operator= (const BaseVector& other) = 0;

  virtual Type DotWith(const BaseVector &other) = 0;
  virtual BaseVector<Type, K> Sub(size_t start_index, size_t end_index=0) = 0;
  virtual BaseVector<Type, K>& SetZero() = 0;
  virtual BaseVector<Type, K>& Randomize() = 0;
  virtual const void* Data() const = 0;
  virtual size_t Size() const = 0;
  virtual std::string DebugString() const = 0;
  // get i-th element
  virtual Type Get(size_t i) const = 0;
}; // class BaseVector

template<class Type, KernelType K>
using VectorPtr = std::unique_ptr<BaseVector<Type, K>>;

// type helpers to make type definitions more convinient
template<class Type>
using CpuBaseVector = BaseVector<Type, CPU>;

template<class Type>
using GpuBaseVector = BaseVector<Type, GPU>;

using FloatCpuBaseVector = CpuBaseVector<float>;
using Int32CpuBaseVector = CpuBaseVector<int32_t>;
/*
template<class Type, KernelType K>
Status NewVector(VectorPtr<Type, K>& v);
*/

} // namespace chasky
#endif
