#ifndef CHASKY_CORE_FRAMEWORK_ARGUMENT_FIELD_H_
#define CHASKY_CORE_FRAMEWORK_ARGUMENT_FIELD_H_
#include <string>
#include <memory>
#include <vector>
#include "chasky/common/string_piece.h"
#include "chasky/common/ref_count.h"
#include "chasky/core/common/status.h"
#include "chasky/core/common/matrix.h"
#include "chasky/core/common/vector.h"
#include "chasky/core/common/eigen_vector.h"
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/framework/kernel.h"
#include "chasky/core/framework/argument.pb.h"
namespace chasky {
#define TYPE_GETTER(NAME, TYPE)                                                \
  std::shared_ptr<TYPE> NAME;                                                  \
  bool get(TYPE **x) {                                                         \
    *x = NAME.get();                                                           \
    return *x != nullptr;                                                      \
  }                                                                            \
  bool is_valid(TYPE *x) const { return NAME.get() != nullptr; }               \
  void reset(std::shared_ptr<TYPE> x) { NAME = x; }                            \
  void create_##NAME() { NAME.reset(new TYPE); };
// Special methods for matrix
#define TYPE_GETTER_MATRIX(NAME, TYPE)                                         \
  void create_##NAME(const ArgumentDef::Shape &shape) {                        \
    reset(std::make_shared<TYPE>(                                              \
        std::make_pair(shape.width(), shape.height())));                       \
  }

// Storage of all kinds of data types, each Argument will have an ArgumentField
// to store its value.
// ArgumentField only support local memory storage.
struct ArgumentField {
  ArgumentField() {}

  // Copy pointer
  void CloneFrom(const ArgumentField &other);

  // Copy memory
  void CopyFrom(const ArgumentField &other);

  bool IsEmpty() const;

  // single argument
  TYPE_GETTER(string_val, std::string);
  TYPE_GETTER(int64_val, int64_t);
  TYPE_GETTER(uint64_val, uint64_t);
  TYPE_GETTER(float_val, float);
  TYPE_GETTER(float_mat_val, math::CpuFloatMatrix);
  TYPE_GETTER_MATRIX(float_mat_val, math::CpuFloatMatrix);
  // diffent types of raw array
  TYPE_GETTER(string_vals, std::vector<std::string>);
  TYPE_GETTER(int64_vals, std::vector<int64_t>);
  TYPE_GETTER(uint64_vals, std::vector<uint64_t>);
  TYPE_GETTER(float_vals, std::vector<float>);
  // Use shapred_ptr to pass in arguments from other functions's output.
  TYPE_GETTER(float_mat_vals,
              std::vector<std::shared_ptr<math::CpuFloatMatrix>>)
  // To support variadic number of arguments.
  // TYPE_GETTER(float_vec_list, std::vector<math::CpuFloatVector>);
}; // struct Argument
#undef TYPE_GETTER

typedef std::shared_ptr<ArgumentField> ArgFldPtr;

// #define ARGUMENT_TYPE_GETTER(NAME, TYPE)                                       \
//   std::shared_ptr<TYPE> NAME##() { return arg_field_->##NAME; }
}
#endif
