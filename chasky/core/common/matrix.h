#ifndef CHASKY_CORE_COMMON_MATRIX_H_
#define CHASKY_CORE_COMMON_MATRIX_H_
#include <memory>
#include "chasky/core/framework/kernel.h"
namespace chasky {

template <class Type, KernelType K> class BaseMatrix {
public:
  // matrix's shape, with first as the number of rows, the second as the number
  // of columns.
  typedef std::pair<size_t, size_t> shape_t;

  virtual BaseMatrix &operator=(const BaseMatrix &other) = 0;

  // multiply `this` with `other`, (this * ratio1) multiply (other * ratio2)
  virtual void MultWith(const BaseMatrix &other, float ratio1,
                                       float ratio2) = 0;

  virtual const Type *Data() const = 0;

  virtual const void *RawMat() const = 0;

  virtual shape_t Shape() const = 0;

  virtual std::string DebugString() const = 0;

  // get element on i-th row j-th col
  virtual Type Get(size_t i, size_t j) const = 0;

}; // class BaseMatrix

} // namespace chasky
#endif
