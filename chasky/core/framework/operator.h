#ifndef CHASKY_CORE_FRAMEWORK_OPERATOR_H_
#define CHASKY_CORE_FRAMEWORK_OPERATOR_H_
#include <vector>
#include <memory>
#include <unordered_map>
#include "chasky/core/common/status.h"
#include "chasky/core/framework/argument.h"
#include "chasky/core/framework/operator.pb.h"
namespace chasky {

// Interface of Operator, operator only support two methods:
// ForwardCompute, which is like ANN's forward; GradCompute, which
// is like ANN's backward
class OperatorInterface {
public:
  virtual Status ForwardCompute() = 0;

  virtual Status GradCompute() = 0;
};

// Execution context for an operator. Includes operator's inputs and outputs,
// Models' parameters.
class ExecContext {
public:
  explicit ExecContext() {}
  explicit ExecContext(const OperatorDef& def);
  // Add a persistent model parameter
  Status AddPersistentArgument(const std::string &name);

  // The persistent arguments may be shared across the whole graph. So use a
  // shared_ptr to share the pointer.
  void SetPersistentArguents(
      std::shared_ptr<std::unordered_map<string, ArgumentField>> args) {
    persistent_argument_ = args;
  }

  // Create arguments from operator def
  Status CreateFromDef(const OperatorDef& def);

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

// Base class for all operators
class Operator : public OperatorInterface {
public:
  void SetExecContext(ExecContext *context);

private:
  ExecContext *exec_context_;
};

class OperatorLibrary {
public:
  using OperatorCreatorType =
      std::function<std::unique_ptr<Operator*>()>;

  // Singleton, the global operator library
  static OperatorLibrary& Instance();

  // Register an operator creator with a name, if another operator called the
  // same name exists, then it will not insert the new operator and just
  // return error.
  // NOTE It is not thread-safe.
  Status Register(const std::string &name, OperatorCreatorType &&creator);

  // Look up the library for the operator which match the name, if not exits,
  // just return error.
  // NOTE It is not Rewrie-Read thread-sefe.
  Status LookUp(const std::string &name, OperatorCreatorType *creator);

  // Human readable description of all the operator creator store in
  // the library.
  std::string DebugString() const;

  // Number of operator creators in the library
  size_t Size() const;

private:
  std::unordered_map<std::string, OperatorCreatorType> op_library_;
};

#define REGISTER_OP(NAME, CREATOR) \
  OperatorLibrary::Instance().Register(NAME, CREATOR)

}

#endif
