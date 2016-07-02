#include "chasky/common/strings.h"
#include "chasky/core/framework/function.h"
using namespace std;

namespace chasky {

/*void Function::SetExecContext(ExecContext *context) {
  exec_context_ = context;
  // CHECK(CheckInput());
  }*/

/*
FunctionLibrary &Instance() {
  static FunctionLibrary *library = new FunctionLibrary();
  return *library;
}
*/

void ArgumentCreateFloat(Argument *arg, ArgumentDef::Shape &shape) {
  size_t width = shape.width();
  arg->ArgField()->float_vals = make_shared<vector<float>>(width);
}

Status FunctionLibrary::Register(const string &name,
                                 FunctionCreatorType &&creator) {
  if (op_library_.count(name) != 0) {
    return Status(
        error::INVALID_ARGUMENT,
        strings::Printf("A key called %s already exists", name.c_str()));
  }
  if (!op_library_.insert({name, creator}).second) {
    return Status(error::UNKNOWN, strings::empty_string);
  }
  return Status();
}

Status FunctionLibrary::LookUp(const string &name,
                               FunctionCreatorType *creator) {
  auto it = op_library_.find(name);
  if (it == op_library_.end()) {
    return Status(error::OUT_OF_RANGE,
                  strings::Printf("no key called %s is found", name.c_str()));
  }
  creator = &it->second;
  return Status();
}

string FunctionLibrary::DebugString() const {
  string res;
  strings::Appendf(&res, "FunctionLibrary with %lu creators\n", Size());
  for (auto item : op_library_) {
    strings::Appendf(&res, "key: [%s]", item.first.c_str());
  }
  return res;
}

size_t FunctionLibrary::Size() const { return op_library_.size(); }

}
