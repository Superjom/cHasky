#include <vector>
#include <unordered_map>
#include <glog/logging.h>

#include "chasky/core/framework/argument.h"
#include "chasky/core/framework/argument_def_builder.h"

using namespace std;

namespace chasky {
Argument::Argument(const Argument &other) : valid_(false) { Assign(other); }

Argument::Argument(const std::string name, const std::string dtype,
                   uint64_t width, uint64_t height)
    : arg_field_(std::make_shared<ArgumentField>()), valid_(false) {
  auto def = ArgumentDefBuilder()
                 .Name(name)
                 .Shape(width, height)
                 .Type(dtype)
                 .Finalize();
  CH_CHECK_OK(FromDef(&def));
}

Status Argument::FromBuffer(const string &buffer) {
  Status status;
  CHECK(!ArgField()->IsEmpty());
  CHECK(!buffer.empty());

  return status;
}

Status Argument::SetList(std::vector<ArgumentPtr> &list) {
  Status status;
  // check all the input arguments have the same dtype
  CHECK(!list.empty());
  auto input_dtype = arg_def_.dtype();

  for (auto &arg : list) {
    CH_STEST_RETURN2(arg->ArgDef()->dtype() == input_dtype,
                     error::INVALID_ARGUMENT,
                     "All the input arguments should have the same dtype");
    CH_CHECK_OK(AppendList(input_dtype, *arg));
  }
  return status;
}

Status Argument::Serialize(ArgumentDef *buffer) const {
  Status status;

  std::string buf;
  switch (arg_def_.dtype()) {
  case DataType::CH_MAT_FLOAT: {
    math::CpuFloatMatrix *mat;
    ArgField()->get(&mat);
    mat->ToBuffer(buf);
  } break;
  default:
    UN_IMPLEMENTED
  }

  *buffer = arg_def_;
  buffer->set_content(buf);
  return status;
}

Status Argument::DeSerialize(const ArgumentDef &buffer) {
  Status status;
  // CHECK(arg_def_.IsInitialized());

  DLOG(INFO) << "buffer: " << buffer.DebugString();
  DLOG(INFO) << "to copy buffer";
  arg_def_ = buffer;
  // leave content field empty to save memory
  arg_def_.clear_content();

  DLOG(INFO) << "fill argument's mat";
  switch (arg_def_.dtype()) {
  case DataType::CH_MAT_FLOAT: {
    math::CpuFloatMatrix *mat;
    ArgField()->get(&mat);
    mat->FromBuffer(buffer.content());
  } break;
  default:
    UN_IMPLEMENTED
  }

  return status;
}

Status Argument::AppendList(DataType dtype, Argument &arg) {
  Status status;
  if (dtype == CH_MAT_FLOAT_LIST) {
    CH_STEST_RETURN2(arg_field_->float_mat_vals, error::NOT_INITED,
                     "float_mat_vals is not inited");
    CH_STEST_RETURN2(
        arg_field_->float_vals, error::INVALID_ARGUMENT,
        "arg's float matrix field is empty, should be inited first");
    arg_field_->float_mat_vals->push_back(arg.ArgField()->float_mat_val);
  }
  return status;
}

const string &Argument::Name() const { return arg_def_.name(); }

const ArgumentDef::Shape &Argument::Shape() const { return arg_def_.shape(); }

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
      {"float_mat", CH_MAT_FLOAT},
      {"float_mat_list", CH_MAT_FLOAT_LIST},
      {"float", CH_MAT_FLOAT_LIST},
      {"int64", CH_INT64}};
  LOG(INFO) << "string2dtype " << type;
  auto it = str2dtype.find(type.tostring());
  CHECK(it != str2dtype.end()) << "String2Dtype not support type [" << type
                               << "]";
  return it->second;
}

// ref or copy ? should ref create an empty matrix?
Status Argument::FromDef(const ArgumentDef *def) {
  Status status;
  CHECK(def);
  DLOG(INFO) << "create Argument from def:\n" << def->DebugString();
  CH_STEST_RETURN2(def != nullptr, error::INVALID_ARGUMENT, "def is nullptr");

  arg_def_ = *def;
  // if (IsRef())
  //   return Status();

  CH_STEST_RETURN2(!arg_def_.type().empty(), error::INVALID_ARGUMENT,
                   "def.type is not set");
  arg_def_.set_dtype(String2Dtype(arg_def_.type()));

  DLOG(INFO) << "init from type " << arg_def_.type() << " " << arg_def_.dtype();

  CHECK(arg_field_);
  CHECK(arg_field_->IsEmpty());

  // copy mode, create its own ArgumentField
  auto &shape = def->shape();
  switch (def->dtype()) {
  // single element
  case CH_FLOAT:
    arg_field_->create_float_val();
    break;
  case CH_INT64:
    arg_field_->create_int64_val();
    break;
  case CH_UINT64:
    arg_field_->create_uint64_val();
    break;
  case CH_STRING:
    arg_field_->create_string_val();
    break;
  case CH_MAT_FLOAT: {
    DLOG(INFO) << "create cpu float matrix";
    arg_field_->create_float_mat_val(shape);
  } break;
  case CH_MAT_FLOAT_LIST: {
    DLOG(INFO) << "create cpu float matrix list";
    arg_field_->create_float_mat_vals();
  } break;
  default:
    status.Update(error::INVALID_ARGUMENT, "not supported type in Argument: %d",
                  (int)def->dtype());
    break;
  }
  return status;
}

inline void Argument::Assign(const Argument &other) {
  CHECK(arg_def_.IsInitialized()) << "arg_def_ should be inited before assign";
  CHECK(other.arg_field_) << "can not copy from null arg";
  DLOG(INFO) << "argument copy assign in ref mode? " << IsRef();
  arg_def_ = *other.ArgDef();

  if (IsRef()) {
    arg_field_ = other.ArgField();
  } else {
    arg_field_ = std::make_shared<ArgumentField>();
    if (arg_def_.is_ref()) {
      arg_field_->CloneFrom(*other.ArgField());
    } else {
      DLOG(INFO) << "copy argument field";
      CHECK(!other.ArgField()->IsEmpty());
      arg_field_->CopyFrom(*other.ArgField());
    }
  }
}

bool Argument::IsList(chasky::DataType dtype) {
  return dtype == CH_MAT_FLOAT_LIST;
}

std::string Argument::Description() const {
  std::stringstream ss;
  ss << std::endl;
  ss << "Argument [" << arg_def_.name() << " ]" << std::endl;
  ss << "    type: " << arg_def_.type() << " " << arg_def_.dtype() << std::endl;
  ss << "    shape: " << arg_def_.shape().width() << " "
     << arg_def_.shape().height() << std::endl;
  ss << std::endl;
  return ss.str();
}
}
