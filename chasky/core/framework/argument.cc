#include <vector>
#include <glog/logging.h>
#include "chasky/core/framework/argument.h"
using namespace std;

namespace chasky {
void ArgumentField::CopyFrom(const ArgumentField &other, bool is_ref) {
  if (is_ref) {
    float_vec_val = other.float_vec_val;
  } else { // copy mode
    LOG(FATAL) << "NotImplemented";
  }
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

Argument::Argument(const Argument &other) : valid_(false) {
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

DataType String2Dtype(StringPiece type) {
  // TODO add more types
  if (type == "vec_float")
    return CH_VEC_FLOAT;
  return CH_VEC_FLOAT;
}

Status Argument::FromDef(const ArgumentDef *def) {
  CHECK(arg_def_ != nullptr);
  arg_def_ = const_cast<ArgumentDef *>(def);
  if (IsRef())
    return Status();

  arg_def_->set_dtype(String2Dtype(arg_def_->type()));

  DLOG(INFO) << "init from type " << arg_def_->type() << " "
             << arg_def_->dtype();

  CHECK(arg_field_);
  CHECK(arg_field_->IsEmpty());

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
  case CH_VEC_FLOAT: {
    DLOG(INFO) << "create cpu float vec";
    arg_field_->float_vec_val =
        make_shared<math::CpuFloatVector>(shape.width());
  } break;
  case CH_VEC_FLOAT_LIST:
    arg_field_->float_vec_list_val =
        make_shared<vector<math::CpuFloatVector>>();
    break;
  default:
    break;
  }
  return Status();
}

/*
// Only need to manage memory in copy mode.
void Argument::RefFree() {
  LOG(INFO) << "free";
  if (IsCopied() && arg_field_) {
    delete arg_field_;
    arg_field_ = nullptr;
  }
}
*/
}
