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
#include "chasky/core/framework/argument_field.h"
namespace chasky {
using std::string;

class Argument;
using ArgumentPtr = std::shared_ptr<Argument>;

class Argument {
public:
  // A default constructor used for new
  explicit Argument()
      : arg_field_{std::make_shared<ArgumentField>()}, valid_(false) {}
  // Init and allocate parameter from Argument Def
  explicit Argument(const ArgumentDef *def)
      : arg_def_(*def), arg_field_(std::make_shared<ArgumentField>()),
        valid_(false) {
    CH_CHECK_OK(FromDef(&arg_def_));
  }

  explicit Argument(const Argument &other);

  explicit Argument(const std::string name, const std::string dtype,
                    uint64_t width = 0, uint64_t height = 0);

  void Assign(const Argument &other);

  void CloneFrom(Argument &other) {
    arg_def_ = other.arg_def_;
    arg_field_->CloneFrom(*other.arg_field_);
    valid_ = other.valid_;
    signature_ = other.signature_;
  }

  bool operator==(const Argument &other);

  // Create ArgumentField from arg_def_
  Status FromDef(const ArgumentDef *def);

  // Init from protobuf buffer, with serialized content.
  Status FromBuffer(const std::string &buffer);

  // Set list-field from a list of arguments.
  // NOTE all the input arguments should be the same dtype.
  Status SetList(std::vector<ArgumentPtr> &list);

  // Serialize the argument to protobuf string. Can be used to save model
  // parameters to file.
  // void DataToString(std::string *buffer) const;

  // Serialize the argument to a ArgumentDef protobuf buffer.
  Status Serialize(ArgumentDef *buffer) const;

  // Set argument from a serialized protobuf buffer.
  Status DeSerialize(const ArgumentDef &buffer);

  // Append an Argument to this.list field.
  // @dtype: list field's dtype
  // @arg: the argument to push_back to the list field
  Status AppendList(DataType dtype, Argument &arg);

  const string &Name() const;

  const ArgumentDef::Shape &Shape() const;

  const ArgumentDef *ArgDef() const { return &arg_def_; }

  // The fields can be accessed by DataType like:
  //     Argument arg;
  //     float *x;
  //     arg.ArgField()->Get(&x);
  const ArgFldPtr ArgField() const { return arg_field_; }
  ArgFldPtr ArgField() { return arg_field_; }

  // set to zero vector or matrix
  void SetZero();

  Argument &operator+=(const Argument &other);

  // Whether this argument is passed by reference, if reference, it will not
  // just copy a pointer from others, otherwise, it will create a ArgumentField
  // and manange the memory, if reference, it will not
  // just copy a pointer from others, otherwise, it will create a ArgumentField,
  // copy content from others and manange ArgumentField's memory
  bool IsRef() const { return arg_def_.is_ref(); }

  bool IsCopied() const { return !IsRef(); };

  void SetValid(bool x) { valid_ = x; }
  bool Valid() const { return valid_; }

  void SetSignature(const std::string &x) { signature_ = x; }
  const std::string &Signature() const { return signature_; }

  // Human-readable short debug string.
  std::string Description() const;

  // Human-readable long debug string.
  std::string DebugString() const;

  // return true if dtype is a list.
  static bool IsList(DataType dtype);

protected:
  Argument &operator=(const Argument &other);

private:
  // Just a pointer to other's def, need not free the memory.
  ArgumentDef arg_def_;
  // If the argument is a reference, arg_field_ will just be a pointer and need
  // not memory management.
  // If the arguemnt is a copy, It will create its own ArgumentField and manage
  // memory.
  ArgFldPtr arg_field_;

  bool valid_;

  // Format like "{node}:{arg_name}".
  std::string signature_;
};

inline bool Argument::operator==(const Argument &other) {
  return ArgField() == other.ArgField();
}

DataType String2Dtype(StringPiece type);

} // namespace chasky
#endif
