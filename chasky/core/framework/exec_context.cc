#include "chasky/core/framework/exec_context.h"

namespace chasky {
// TODO Add more check
bool ExecContext::IsValid() const { return persistent_argument_ != nullptr; }

Status ExecContext::CreateFromDef(const chasky::OperatorDef &def) {
  // TODO prepare inputs
  // prepare outputs, allocate memory and hold it
  return Status();
}

Status ExecContext::AddInput(const std::string &key, const ArgumentDef &def) {
  input_args_.emplace(key, Argument(&def));
  return Status();
}

Status ExecContext::AddOutput(const std::string &key, const ArgumentDef &def) {
  output_args_.emplace(key, Argument(&def));
  return Status();
}

void ExecContext::WaitUntilForwardReady() {
  std::unique_lock<std::mutex> lock(mu_);
  forward_ready_cond_.wait(lock, [this] {
    for (const auto &item : input_args_) {
      if (!item.second.Valid())
        return false;
    }
    return true;
  });
}

void ExecContext::WaitUntilBackwardReady() {
  std::unique_lock<std::mutex> lock(mu_);
  forward_ready_cond_.wait(lock, [this] {
    for (const auto &item : grad_args_) {
      if (!item.second.Valid())
        return false;
    }
    return true;
  });
}
}
