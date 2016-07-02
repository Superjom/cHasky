// exec_context.h
// Created Date: 2016-06-18
#ifndef CHASKY_CORE_FRAMEWORK_EXEC_CONTEXT_H_
#define CHASKY_CORE_FRAMEWORK_EXEC_CONTEXT_H_
#include <memory>
#include <unordered_map>
#include "chasky/core/common/status.h"
#include "chasky/core/framework/argument.h"
#include "chasky/core/framework/function.pb.h"
namespace chasky {

// Execution context for an operator. Includes operator's inputs and outputs,
// Models' parameters.
// To execute an operator, its ExecContext should be readly first.
class ExecContext {
public:
  explicit ExecContext() {}
  explicit ExecContext(const OperatorDef &def);
  // Add a persistent model parameter
  Status AddPersistentArgument(const std::string &name);

  // The persistent arguments may be shared across the whole graph. So use a
  // shared_ptr to share the pointer.
  void SetPersistentArguents(
      std::shared_ptr<std::unordered_map<string, ArgumentField>> args) {
    persistent_argument_ = args;
  }

  // Create arguments from operator's defifintion. Will create inputs
  // and outputs
  Status CreateFromDef(const OperatorDef &def);

  Status AddInput(const std::string &key, const ArgumentDef &def);

  // set argument and make it valid
  Status AddInput(const std::string &key, Argument arg) {
    input_args_[key] = arg;
    input_args_[key].SetValid(true);
    std::unique_lock<std::mutex> lock(mu_);
    forward_ready_cond_.notify_all();
    return Status();
  }

  Status AddGrad(const std::string &key, Argument arg) {
    grad_args_[key] = arg;
    grad_args_[key].SetValid(true);
    std::unique_lock<std::mutex> lock(mu_);
    backward_ready_cond_.notify_all();
    return Status();
  }

  Status AddOutput(const std::string &key, const ArgumentDef &def);

  const Argument &input(const std::string &key) { return input_args_[key]; }
  const Argument &output(const std::string &key) { return output_args_[key]; }
  size_t inputs_size() const { return input_args_.size(); }
  size_t output_size() const { return output_args_.size(); }
  Argument *mutable_input(const std::string &key) { return &input_args_[key]; }
  Argument *mutable_output(const std::string &key) {
    return &output_args_[key];
  }

  void SetReady(const std::string &arg_key) {
    input_args_[arg_key].SetValid(true);
  }

  bool IsValid() const;

  // Wait for all the input activation arguments are ready.
  void WaitUntilForwardReady();

  // Wait for all the input gradient arguments are ready.
  void WaitUntilBackwardReady();

  bool KeepRunning() const { return *keep_running_; }

  const std::unordered_map<std::string, Argument> &InputArgs() const {
    return input_args_;
  }
  const std::unordered_map<std::string, Argument> &OutputArgs() const {
    return output_args_;
  }

  std::mutex &Lock() { return mu_; }
  std::condition_variable &ForwardReadyCond() { return forward_ready_cond_; }
  std::condition_variable &BackwardReadyCond() { return backward_ready_cond_; }

private:
  // key is "%s:%s:%s" % (node.name, "input", arg.name)
  std::unordered_map<std::string, Argument> input_args_;

  // key is "%s:%s:%s" % (node.name, "output", arg.name)
  std::unordered_map<std::string, Argument> output_args_;

  // key is "%s:%s:%s" % (node.name, "grad", arg.name)
  std::unordered_map<std::string, Argument> grad_args_;

  // Persistent arguments(model's parameters)
  std::shared_ptr<std::unordered_map<std::string, ArgumentField>>
      persistent_argument_;

  // global flag tells whether to keep running
  bool *keep_running_;

  mutable std::mutex mu_;
  mutable std::condition_variable forward_ready_cond_;
  mutable std::condition_variable backward_ready_cond_;
};
}
#endif
