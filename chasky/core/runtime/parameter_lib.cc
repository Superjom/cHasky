#include "chasky/core/common/status.h"
#include "chasky/core/runtime/parameter_lib.h"
namespace chasky {

Status ParameterLib::RegisterCreate(const string &key, const ArgumentDef &def) {
  Status status;
  auto it = params_.find(key);
  if (it != params_.end()) {
    status.Update(
        error::INVALID_ARGUMENT,
        strings::Printf("duplicate register parameter called %s", key.c_str()));
  } else if (!params_.insert(
                         std::make_pair(key, std::make_shared<Argument>(&def)))
                  .second) {
    status.Update(error::UNKNOWN,
                  strings::Printf("parameter %s insert error", key.c_str()));
  }
  return status;
}

Status ParameterLib::Retrieve(const string &key, ArgumentPtr *param) const {
  Status status;
  auto it = params_.find(key);
  CH_STEST_RETURN2(it != params_.end(), error::OUT_OF_RANGE,
                   "retrieve parameter called %s failed. no such key.",
                   key.c_str());
  *param = it->second;
  return status;
}

}; // namespace chasky
