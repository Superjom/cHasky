#ifndef CHASKY_CORE_FRAMEWORK_ARGUMENT_H_
#define CHASKY_CORE_FRAMEWORK_ARGUMENT_H_
#include <string>
#include <memory>
#include <vector>
#include "chasky/common/string_piece.h"
#include "chasky/common/ref_count.h"
#include "chasky/core/common/status.h"
#include "chasky/core/common/matrix.h"
#include "chasky/core/common/vector.h"
#include "chasky/core/common/eigen_vector.h"
#include "chasky/core/framework/kernel.h"
#include "chasky/core/framework/argument.pb.h"
namespace chasky {
using std::string;

#define TYPE_GETTER(NAME, TYPE) std::shared_ptr<TYPE> NAME##_val;

#define TYPE_VALS_GETTER(NAME, TYPE)                                           \
  std::shared_ptr<std::vector<TYPE>> NAME##_vals;                              \
  void Create##NAME(const ArgumentDef::Shape &shape) {                         \
    NAME##_vals = std::make_shared<std::vector<TYPE>>(shape.width());          \
  }

// Storage of all kinds of data types, each Argument will have an ArgumentField
// to store its value.
// ArgumentField only support local memory storage.
struct ArgumentField {
  ArgumentField() {}

  // Copy pointer
  void CopyFrom(const ArgumentField &other, bool is_ref);

  // Copy memory
  void RealCopyFrom(const ArgumentField &other);

  bool IsEmpty() const;

  // diffent types of raw array
  TYPE_VALS_GETTER(int32, int32_t);
  TYPE_VALS_GETTER(int64, int64_t);
  TYPE_VALS_GETTER(uint32, uint32_t);
  TYPE_VALS_GETTER(uint64, uint64_t);
  TYPE_VALS_GETTER(float, float);
  TYPE_VALS_GETTER(double, double);
  // vector getter
  TYPE_GETTER(float_vec, math::CpuFloatVector);
  // TYPE_GETTER(int32_vec, Int32CpuBaseVector);
  // store string variables, not document
  TYPE_GETTER(string, std::string);
  // NOTE(superjom) just a useless placeholder
  // std::shared_ptr<BaseMatrix> matrix_val_;

  // To support variadic number of arguments.
  TYPE_GETTER(float_vec_list, std::vector<math::CpuFloatVector>);
}; // struct Argument
#undef TYPE_GETTER
#undef TYPE_VALS_GETTER

typedef std::shared_ptr<ArgumentField> ArgFldPtr;

class Argument {
public:
  // A default constructor used for new
  explicit Argument()
      : arg_def_(nullptr), arg_field_{std::make_shared<ArgumentField>()},
        valid_(false) {}
  // Init and allocate parameter from Argument Def
  explicit Argument(const ArgumentDef *def)
      : arg_def_(const_cast<ArgumentDef *>(def)), valid_(false),
        arg_field_(std::make_shared<ArgumentField>()) {
    CH_CHECK_OK(FromDef(arg_def_));
  }

  explicit Argument(const Argument &other);

  Argument &operator=(const Argument &other);
  bool operator==(const Argument &other);

  // Create ArgumentField from arg_def_
  Status FromDef(const ArgumentDef *def);

  // Init from protobuf buffer.
  Status FromProto(const std::string &buffer);

  // Serialize the argument to protobuf string. Can be used to save model
  // parameters to file.
  void ToProto(std::string *buffer) const;

  const string &Name() const;

  const ArgumentDef::Shape &Shape() const;

  const ArgumentDef *ArgDef() const { return arg_def_; }

  const ArgFldPtr ArgField() const { return arg_field_; }

  // set to zero vector or matrix
  void SetZero();

  Argument &operator+=(const Argument &other);

  // Whether this argument is passed by reference, if reference, it will not
  // just copy a pointer from others, otherwise, it will create a ArgumentField
  // and manange the memory, if reference, it will not
  // just copy a pointer from others, otherwise, it will create a ArgumentField,
  // copy content from others and manange ArgumentField's memory
  bool IsRef() const { return arg_def_->is_ref(); }

  bool IsCopied() const { return !IsRef(); };

  void SetValid(bool x) { valid_ = x; }
  bool Valid() const { return valid_; }

protected:
private:
  // Just a pointer to other's def, need not free the memory.
  ArgumentDef *arg_def_;
  // If the argument is a reference, arg_field_ will just be a pointer and need
  // not memory management.
  // If the arguemnt is a copy, It will create its own ArgumentField and manage
  // memory.
  ArgFldPtr arg_field_;

  bool valid_;
};

typedef std::shared_ptr<Argument> ArgumentPtr;

inline Argument &Argument::operator=(const Argument &other) {
  CHECK(arg_def_) << "arg_def_ should be inited before assign";
  CHECK(other.ArgField()) << "can not copy from null arg";
  DLOG(INFO) << "argument copy assign in ref mode? " << IsRef();

  if (IsRef()) {
    arg_field_ = other.ArgField();
  } else {
    arg_field_ = std::make_shared<ArgumentField>();
    arg_field_->CopyFrom(*other.ArgField(), arg_def_->is_ref());
  }
  return *this;
}

inline bool Argument::operator==(const Argument &other) {
  return ArgField() == other.ArgField();
}

} // namespace chasky
#endif
