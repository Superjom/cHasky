#include "chasky/core/framework/operator_def_builder.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/attr_value_util.h"
using namespace std;

namespace chasky {

// Set default width for each output
OperatorDef &OperatorDefBuilder::Finalize() {
  size_t default_width = 0;
  if (def_.inputs_size() > 0) {
    default_width = def_.inputs(0).shape().width();
  }
  for (size_t i = 0; i < def_.outputs_size(); i++) {
    if (def_.outputs(i).shape().width() == 0) {
      def_.mutable_outputs(i)->mutable_shape()->set_width(default_width);
    }
  }
  return def_;
}

Status OperatorDefLibrary::Register(const string &name, OperatorDef &&def) {
  if (def_library_.count(name) != 0) {
    return Status(error::INVALID_ARGUMENT,
                  strings::Printf("An operator called %s has been registered",
                                  name.c_str()));
  }
  if (!def_library_.insert({name, def}).second) {
    return Status(error::UNKNOWN, strings::empty_string);
  }

  return Status();
}

Status OperatorDefLibrary::LookUp(const string &name, const OperatorDef *def) {
  auto it = def_library_.find(name);
  if (it == def_library_.end()) {
    return Status(
        error::OUT_OF_RANGE,
        strings::Printf("no operator definition called %s.", name.c_str()));
  }
  def = &it->second;
  return Status();
}

string OperatorDefLibrary::DebugString() const {
  string res =
      strings::Printf("OperatorDefLibrary size: %lu\n", def_library_.size());
  for (const auto &item : def_library_) {
    strings::Appendf(&res, "%s : \n%s\n", item.first.c_str(),
                     item.second.DebugString().c_str());
  }
  return res;
}

}
