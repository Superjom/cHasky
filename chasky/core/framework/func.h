#ifndef CHASKY_CORE_FRAMEWORK_OPERATOR_H_
#define CHASKY_CORE_FRAMEWORK_OPERATOR_H_
#include <vector>
#include <memory>
#include <unordered_map>
#include "chasky/core/common/status.h"
#include "chasky/common/string_piece.h"
#include "chasky/core/framework/argument.h"
#include "chasky/core/framework/exec_context.h"
#include "chasky/core/framework/operator.pb.h"
#include "chasky/core/framework/graph.pb.h"
namespace chasky {

// Interface of Operator, operator only support two methods:
// ForwardCompute, which is like ANN's forward; GradCompute, which
// is like ANN's backward
class OperatorInterface {
public:
  virtual Status ForwardCompute() = 0;

  virtual Status GradCompute() = 0;
};

// Base class for all operators
class Operator : public OperatorInterface {
public:
  void SetExecContext(ExecContext *context);

  virtual void CheckContext() = 0;

  // Create an operator from definition
  // @def: operator's definition
  // @attrs: node's attrs
  // The operator's attributes is registered in `attrs`, this api should read
  // definition's field names and try to get value from `attrs`.
  virtual Status
  FromDef(const OperatorDef &def,
          const ::google::protobuf::Map<::std::string, ::chasky::AttrValue>
              &attrs) = 0;

  StringPiece Name() const { return name_; }

  const OperatorDef &Def() const { return def_; }

  const ExecContext &exec_context() { return *exec_context_; }
  ExecContext *mutable_exec_context() { return exec_context_; }

private:
  ExecContext *exec_context_;
  StringPiece name_;
  OperatorDef def_;
};

class OperatorLibrary {
public:
  using OperatorCreatorType = std::function<std::unique_ptr<Operator>()>;

  // Singleton, the global operator library
  static OperatorLibrary &Instance() {
    static OperatorLibrary *library = new OperatorLibrary();
    return *library;
  }

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

#define REGISTER_OP(NAME, CREATOR)                                             \
  OperatorLibrary::Instance().Register(NAME, CREATOR)
}

#endif
