// exec_context.h
// Created Date: 2016-06-18
#ifndef CHASKY_CORE_FRAMEWORK_EXEC_CONTEXT_H_
#define CHASKY_CORE_FRAMEWORK_EXEC_CONTEXT_H_
#include <memory>
#include <unordered_map>
#include "chasky/core/common/status.h"
#include "chasky/core/framework/argument.h"
#include "chasky/core/framework/operator.pb.h"
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

  Status AddInput(const ArgumentDef &def);

  Status AddOutpuut(const ArgumentDef &def);

  const Argument &input(size_t i) { return input_args_[i]; }
  const Argument &output(size_t i) { return input_args_[i]; }
  Argument &mutable_input(size_t i) { return input_args_[i]; }
  Argument &mutable_output(size_t i) { return output_args_[i]; }

  bool IsValid() const;

private:
  std::vector<Argument> input_args_;
  std::vector<Argument> output_args_;

  OperatorDef operator_def_;

  // Persistent arguments(model's parameters)
  std::shared_ptr<std::unordered_map<std::string, ArgumentField>>
      persistent_argument_;
};
}
#endif
