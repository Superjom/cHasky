#include "chasky/core/framework/function_def_builder.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/attr_value_util.h"
using namespace std;

namespace chasky {

// Set default width for each output
FunctionDef FunctionDefBuilder::Finalize() {
  size_t default_width = 0;
  for (size_t i = 0; i < def_.inputs_size(); i++) {
    if (def_.inputs(i).has_shape()) {
      default_width = def_.inputs(i).shape().width();
      if (default_width > 0)
        break;
    }
  }
  if (default_width == 0) {
    DLOG(WARNING) << "FunctionDef " << def_.name() << " no shape is set";
    return def_;
  }

  for (size_t i = 0; i < def_.outputs_size(); i++) {
    if (def_.outputs(i).shape().width() == 0) {
      def_.mutable_outputs(i)->mutable_shape()->set_width(default_width);
    }
  }
  return def_;
}

Status FunctionDefLibrary::Register(const string &name, FunctionDef &&def) {
  Status status;
  DLOG(INFO) << ".. begin register def " << name;
  CH_STEST_RETURN2(def_library_.count(name) == 0, error::INVALID_ARGUMENT,
                   "An operator called %s has been registered", name.c_str());

  CH_STEST_RETURN2(def_library_.insert({name, def}).second, error::UNKNOWN,
                   "insert map error");
  return status;
}

Status FunctionDefLibrary::LookUp(const string &name, FunctionDef **def) {
  auto it = def_library_.find(name);
  if (it == def_library_.end()) {
    return Status(
        error::OUT_OF_RANGE,
        strings::Printf("no operator definition called %s.", name.c_str()));
  }
  *def = &it->second;
  return Status();
}

string FunctionDefLibrary::DebugString() const {
  string res =
      strings::Printf("FunctionDefLibrary size: %lu\n", def_library_.size());
  for (const auto &item : def_library_) {
    strings::Appendf(&res, "%s : \n%s\n", item.first.c_str(),
                     item.second.DebugString().c_str());
  }
  return res;
}
}
