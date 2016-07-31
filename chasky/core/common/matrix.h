#ifndef CHASKY_CORE_COMMON_MATRIX_H_
#define CHASKY_CORE_COMMON_MATRIX_H_
#include <memory>
#include "chasky/core/common/status.h"
#include "chasky/core/framework/kernel.h"
namespace chasky {

template <class Type, class RawMatType, KernelType K> class BaseMatrix {
public:
  // matrix's shape, with first as the number of rows, the second as the number
  // of columns.
  typedef std::pair<size_t, size_t> shape_t;

  virtual BaseMatrix &operator=(const BaseMatrix &other) = 0;

  // Multiply `this` with `other`: this = (this * ratio1) * (other * ratio2)
  virtual void MultWith(const BaseMatrix &other, float ratio1,
                        float ratio2) = 0;
  // Add `this` with `other` : this = (this * ratio1) + (other * ratio2)
  virtual void AddWith(const BaseMatrix &other, float ratio1, float ratio2) = 0;

  virtual void Reshape(size_t width, size_t height) = 0;

  // Set all elements to zero.
  virtual void SetZero() = 0;

  // Share data with other, just by pointer.
  virtual void CloneFrom(BaseMatrix &other) = 0;

  // Malloc memory and copy data from other.
  virtual void CopyFrom(BaseMatrix &other) = 0;

  virtual const Type *Data() const = 0;

  virtual const void *RawMat() const = 0;

  virtual std::shared_ptr<RawMatType> MatPtr() const = 0;

  virtual shape_t Shape() const = 0;

  virtual std::string DebugString() const = 0;

  // Human-readable short debug string.
  virtual std::string Description() const = 0;

  // get element on i-th row j-th col
  virtual Type Get(size_t i, size_t j) const = 0;

  virtual std::string Serialize() const = 0;

  virtual Status DeSerialize(const std::string &ss) = 0;

  // protected:
  // std::shared_ptr<RawMatType> mat_;

}; // class BaseMatrix

} // namespace chasky
#endif
