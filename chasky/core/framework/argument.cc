#include <vector>
#include "chasky/core/framework/argument.h"
using namespace std;
using namespace chasky;

void ArgumentField::CopyFrom(const ArgumentField &other) {
  float_vec_val_ = other.float_vec_val();
}

void ArgumentField::RealCopyFrom(const ArgumentField &other) {
  float_vec_val_ = std::make_shared<math::CpuFloatVector>(*other.float_vec_val());
}

bool ArgumentField::IsEmpty() const {
  return int32_vals_ == nullptr && int64_vals_ == nullptr &&
         uint32_vals_ == nullptr && uint64_vals_ == nullptr &&
         float_vals_ == nullptr && double_vals_ == nullptr &&
         float_vec_val_ == nullptr &&
         string_val_ == nullptr;
}

Argument::Argument(const Argument &other) {
  arg_def_.reset(new ArgumentDef(*other.ArgDef()));
  arg_field_.reset(new ArgumentField(*other.ArgField()));
}

Status Argument::FromProto(const string &buffer) {
  LOG(FATAL) << "NotImplemented";
}

void Argument::ToProto(string *buffer) const {
  LOG(FATAL) << "NotImplemented";
}

const string& Argument::Name() const {
  return arg_def_->name();
}

const ArgumentDef::Shape &Argument::Shape() const {
  return arg_def_->shape();
}

// A helper class to create argument according to ArgDef
class Def2Arg {
public:
  using TypeParser = std::function<bool(const string& type, Argument* arg)>;

private:
  vector<TypeParser> parsers_;
};

// NOTE Need not implement?
Status Argument::CreateFromDef() {
  LOG(FATAL) << "NotImplemented";
}

