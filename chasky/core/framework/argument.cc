#include <vector>
#include <unordered_map>
#include <glog/logging.h>
#include "chasky/core/framework/argument.h"
using namespace std;

namespace chasky {
void ArgumentField::CopyFrom(const ArgumentField &other, bool is_ref) {
  if (is_ref) {
    int32_val = other.int32_val;
    uint32_val = other.uint32_val;
    uint64_val = other.uint64_val;
    int64_val = other.int64_val;
    float_mat_val = other.float_mat_val;
    // TODO add all other fields
  } else { // copy mode
    LOG(FATAL) << "NotImplemented";
  }
}

void ArgumentField::RealCopyFrom(const ArgumentField &other) {
  // float_vec_val =
  // std::make_shared<math::CpuFloatVector>(*other.float_vec_val);
}

bool ArgumentField::IsEmpty() const {
  return float_val == nullptr && int32_val == nullptr &&
         uint32_val == nullptr && uint64_val == nullptr &&
         float_mat_val == nullptr && float_mat_vals == nullptr &&
         int32_vals == nullptr && int64_vals == nullptr &&
         uint32_vals == nullptr && uint64_vals == nullptr &&
         float_vals == nullptr && double_vals == nullptr;
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
  // TODO register more types
  static std::unordered_map<std::string, DataType> str2dtype = {
      {"float_mat", CH_MAT_FLOAT}};
  LOG(INFO) << "string2dtype " << type;
  return str2dtype[type.tostring()];
}

Status Argument::FromDef(const ArgumentDef *def) {
  CHECK(def != nullptr);
  CHECK(arg_def_ == nullptr || arg_def_ == def) << "duplicate set arg's definition."
                             << "arg_def_ is already set to " << arg_def_;
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
  // single element
  case CH_FLOAT:
    arg_field_->create_float_val();
    break;
  case CH_DOUBLE:
    arg_field_->create_double_val();
    break;
  case CH_INT32:
    arg_field_->create_int32_val();
    break;
  case CH_INT64:
    arg_field_->create_int64_val();
    break;
  case CH_UINT32:
    arg_field_->create_uint32_val();
    break;
  case CH_UINT64:
    arg_field_->create_uint64_val();
    break;
  case CH_STRING:
    arg_field_->create_string_val();
    break;
  // case CH_VEC_FLOAT: {
  //   DLOG(INFO) << "create cpu float vec";
  //   arg_field_->float_vec_val =
  //       make_shared<math::CpuFloatVector>(shape.width());
  // } break;
  case CH_MAT_FLOAT: {
    DLOG(INFO) << "create cpu float matrix";
    arg_field_->create_float_mat_val(shape);
  } break;
  // case CH_VEC_FLOAT_LIST:
  //   arg_field_->float_vec_list_val =
  //       make_shared<vector<math::CpuFloatVector>>();
  //   break;
  default:
    break;
  }
  return Status();
}
}
