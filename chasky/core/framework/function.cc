#include "chasky/common/strings.h"
#include "chasky/core/framework/function.h"
using namespace std;

namespace chasky {

bool Function::ParseSignature(const std::string &sign, std::string *name,
                                     DataType *dtype) {
  auto pieces = strings::Split(sign, ':');
  if (pieces.size() != 2)
    return false;
  *name = pieces[0];
  *dtype = static_cast<DataType>(std::stoi(pieces[1]));
  return true;
}

void ArgumentCreateFloat(Argument *arg, ArgumentDef::Shape &shape) {
  size_t width = shape.width();
  arg->ArgField()->float_vals = make_shared<vector<float>>(width);
}

Status FunctionLibrary::Register(const string &name,
                                 FunctionCreatorType &&creator) {
  Status status;
  CH_STEST_RETURN2(op_library_.count(name) == 0, error::INVALID_ARGUMENT,
                   "A key called %s already exists", name.c_str());
  CH_STEST_RETURN2(op_library_.insert({name, creator}).second, error::UNKNOWN,
                   "map insert error");
  return status;
}

Status FunctionLibrary::LookUp(const string &name,
                               FunctionCreatorType **creator) {
  auto it = op_library_.find(name);
  if (it == op_library_.end()) {
    return Status(error::OUT_OF_RANGE,
                  strings::Printf("no key called '%s' is found", name.c_str()));
  }
  *creator = &it->second;
  return Status();
}

string FunctionLibrary::DebugString() const {
  string res;
  strings::Appendf(&res, "\n=================================\n");
  strings::Appendf(&res, "FunctionLibrary with %lu creators:\n\n", Size());
  for (auto item : op_library_) {
    strings::Appendf(&res, "key: [%s]", item.first.c_str());
  }
  strings::Appendf(&res, "\n=================================\n");
  return res;
}

size_t FunctionLibrary::Size() const { return op_library_.size(); }
}
