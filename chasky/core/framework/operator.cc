#include "chasky/common/strings.h"
#include "chasky/core/framework/operator.h"
using namespace std;
using namespace chasky;

void Operator::SetExecContext(ExecContext *context) { exec_context_ = context; }

static OperatorLibrary &Instance() {
  static OperatorLibrary *library = new OperatorLibrary();
  return *library;
}

void ArgumentCreateFloat(Argument* arg, ArgumentDef::Shape& shape) {
  size_t width = shape.width();
  arg->ArgField()->float_vals = make_shared<vector<float> >(width);
}

Status OperatorLibrary::Register(const string& name, OperatorCreatorType&& creator) {
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

Status OperatorLibrary::LookUp(const string &name,
                               OperatorCreatorType *creator) {
  auto it = op_library_.find(name);
  if (it == op_library_.end()) {
    return Status(error::OUT_OF_RANGE,
                  strings::Printf("no key called %s is found", name.c_str()));
  }
  creator = &it->second;
  return Status();
}

string OperatorLibrary::DebugString() const {
  string res;
  strings::Appendf(&res, "OperatorLibrary with %lu creators\n", Size());
  for (auto item : op_library_) {
    strings::Appendf(&res, "key: [%s]", item.first.c_str());
  }
  return res;
}

size_t OperatorLibrary::Size() const { return op_library_.size(); }
