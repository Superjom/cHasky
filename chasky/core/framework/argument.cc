#include <vector>
#include "chasky/core/framework/argument.h"
using namespace std;

namespace chasky {
void ArgumentField::CopyFrom(const ArgumentField &other) {
  float_vec_val = other.float_vec_val;
}

void ArgumentField::RealCopyFrom(const ArgumentField &other) {
  float_vec_val = std::make_shared<math::CpuFloatVector>(*other.float_vec_val);
}

bool ArgumentField::IsEmpty() const {
  return int32_vals == nullptr && int64_vals == nullptr &&
         uint32_vals == nullptr && uint64_vals == nullptr &&
         float_vals == nullptr && double_vals == nullptr &&
         float_vec_val == nullptr && string_val == nullptr;
}

Argument::Argument(const Argument &other) {
  arg_def_ = const_cast<ArgumentDef *>(other.ArgDef());
  *this = other;
}

Status Argument::FromProto(const string &buffer) {
  LOG(FATAL) << "NotImplemented";
}

void Argument::ToProto(string *buffer) const { LOG(FATAL) << "NotImplemented"; }

const string &Argument::Name() const { return arg_def_->name(); }

const ArgumentDef::Shape &Argument::Shape() const { return arg_def_->shape(); }

// A helper class to create argument according to ArgDef
class Def2Arg {
public:
  using TypeParser = std::function<bool(const string &type, Argument *arg)>;

private:
  vector<TypeParser> parsers_;
};

Status Argument::FromDef(const ArgumentDef *def) {
  CHECK_NE(arg_def_, nullptr);
  arg_def_ = const_cast<ArgumentDef *>(def);
  if (IsRef())
    return Status();

  // copy mode, create its own ArgumentField
  auto &shape = def->shape();
  switch (def->dtype()) {
  case CH_FLOAT:
    arg_field_->Createfloat(shape);
    break;
  case CH_DOUBLE:
    arg_field_->Createdouble(shape);
    break;
  case CH_INT32:
    arg_field_->Createint32(shape);
    break;
  case CH_INT64:
    arg_field_->Createint64(shape);
    break;
  case CH_UINT32:
    arg_field_->Createuint32(shape);
    break;
  case CH_UINT64:
    arg_field_->Createuint64(shape);
    break;
  case CH_STRING:
    arg_field_->string_val = make_shared<string>();
    break;
  case CH_VEC_FLOAT:
    arg_field_->float_vec_val =

        make_shared<math::CpuFloatVector>(shape.width());
    break;
  case CH_VEC_FLOAT_LIST:
    arg_field_->float_vec_list_val =
        make_shared<vector<math::CpuFloatVector>>();
    break;
  default:
    break;
  }
  return Status();
}

// Only need to manage memory in copy mode.
void Argument::RefFree() {
  if (IsCopied()) {
    delete arg_field_;
    arg_field_ = nullptr;
  }
}
}
