#ifndef CHASKY_CORE_FRAMEWORK_ARGUMENT_H_
#define CHASKY_CORE_FRAMEWORK_ARGUMENT_H_
#include <string>
#include <memory>
#include <vector>
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
    NAME##_vals = std::make_shared<std::vector<TYPE>>(shape.width());            \
  }

// Storage of all kinds of data types, each Argument will have an ArgumentField
// to store its value.
// ArgumentField only support local memory storage.
struct ArgumentField {
  ArgumentField() {}

  // Copy pointer
  void CopyFrom(const ArgumentField &other);

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

class Argument {
public:
  // A default constructor used for new
  explicit Argument() {}
  // Init and allocate parameter from Argument Def
  explicit Argument(const ArgumentDef* arg) :
      arg_def_(const_cast<ArgumentDef*>(arg)) {
    CH_CHECK_OK(FromDef(arg_def_));
  }

  // NOTE Copy memory
  explicit Argument(const Argument &other);

  // Create ArgumentField from arg_def_
  Status FromDef(const ArgumentDef* def);

  // Init from protobuf buffer.
  Status FromProto(const std::string &buffer);

  // Serialize the argument to protobuf string. Can be used to save model
  // parameters to file.
  void ToProto(std::string *buffer) const;

  const string &Name() const;

  const ArgumentDef::Shape &Shape() const;

  const ArgumentDef *ArgDef() const { return arg_def_; }

  ArgumentField *ArgField() { return arg_field_.get(); }
  const ArgumentField *ArgField() const { return arg_field_.get(); }

private:
  ArgumentDef* arg_def_;
  std::unique_ptr<ArgumentField> arg_field_;
};

typedef std::shared_ptr<Argument> ArgumentPtr;

} // namespace chasky
#endif
