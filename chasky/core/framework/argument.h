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
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/framework/kernel.h"
#include "chasky/core/framework/argument.pb.h"
namespace chasky {
using std::string;

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
  void CopyFrom(const ArgumentField &other, bool is_ref);

  // Copy memory
  void RealCopyFrom(const ArgumentField &other);

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

class Argument {
public:
  // A default constructor used for new
  explicit Argument()
      : arg_def_(nullptr), arg_field_{std::make_shared<ArgumentField>()},
        valid_(false) {}
  // Init and allocate parameter from Argument Def
  explicit Argument(const ArgumentDef *def)
      : arg_def_(const_cast<ArgumentDef *>(def)),
        arg_field_(std::make_shared<ArgumentField>()), valid_(false) {
    CH_CHECK_OK(FromDef(arg_def_));
  }

  explicit Argument(const Argument &other);

  void Assign(const Argument &other);

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

  // The fields can be accessed by DataType like:
  //     Argument arg;
  //     float *x;
  //     arg.ArgField()->Get(&x);
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

  void SetSignature(const std::string &x) { signature_ = x; }
  const std::string &Signature() const { return signature_; }

  // Human-readable short debug string.
  std::string Description() const;

  // Human-readable long debug string.
  std::string DebugString() const;

protected:
  Argument &operator=(const Argument &other);

private:
  // Just a pointer to other's def, need not free the memory.
  ArgumentDef *arg_def_;
  // If the argument is a reference, arg_field_ will just be a pointer and need
  // not memory management.
  // If the arguemnt is a copy, It will create its own ArgumentField and manage
  // memory.
  ArgFldPtr arg_field_;

  bool valid_;

  // Format like "{node}:{arg_name}".
  std::string signature_;
};

typedef std::shared_ptr<Argument> ArgumentPtr;

inline void Argument::Assign(const Argument &other) {
  CHECK(arg_def_) << "arg_def_ should be inited before assign";
  CHECK(other.ArgField()) << "can not copy from null arg";
  DLOG(INFO) << "argument copy assign in ref mode? " << IsRef();
  arg_def_ = const_cast<ArgumentDef *>(other.ArgDef());

  if (IsRef()) {
    arg_field_ = other.ArgField();
  } else {
    arg_field_ = std::make_shared<ArgumentField>();
    arg_field_->CopyFrom(*other.ArgField(), arg_def_->is_ref());
  }
}

inline bool Argument::operator==(const Argument &other) {
  return ArgField() == other.ArgField();
}

} // namespace chasky
#endif
