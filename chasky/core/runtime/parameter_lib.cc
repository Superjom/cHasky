#include "chasky/core/common/status.h"
#include "chasky/core/runtime/parameter_lib.h"
namespace chasky {

Status ParameterLib::RegisterCreate(const string &key, const ArgumentDef &def,
                                    ArgumentPtr *param) {
  Status status;
  auto it = params_.find(key);
  CH_STEST_RETURN2(it == params_.end(), error::INVALID_ARGUMENT,
                   "duplicate register parameter [%s]", key.c_str());

  auto new_param = std::make_shared<Argument>(&def);

  CH_STEST_RETURN2(params_.insert(std::make_pair(key, new_param)).second,
                   error::OUT_OF_RANGE,
                   "failed to insert param [%s] to library", key.c_str());

  *param = new_param;
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
