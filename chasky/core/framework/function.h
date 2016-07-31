#ifndef CHASKY_CORE_FRAMEWORK_FUNC_H_
#define CHASKY_CORE_FRAMEWORK_FUNC_H_
#include <vector>
#include <memory>
#include <unordered_map>
#include "chasky/core/common/status.h"
#include "chasky/common/strings.h"
#include "chasky/common/string_piece.h"
#include "chasky/core/framework/argument.pb.h"
#include "chasky/core/framework/argument.h"
//#include "chasky/core/framework/exec_context.h"
#include "chasky/core/framework/function.pb.h"
#include "chasky/core/framework/graph.pb.h"
namespace chasky {

// Interface of Func, func only support two methods:
// ForwardCompute, which is like ANN's forward; GradCompute, which
// is like ANN's backward
class FunctionInterface {
public:
  // A Node will determine which function to use. And a function will init
  // itself according to function's definition(attributes).
  virtual Status InitFromProto(const FunctionDef &def) = 0;

  // Forward computation
  // args: input arguments. If inputs are matrixs, then all their shape should
  // be the same.
  // activation: function's activation output
  // def: function's definition
  // A real function will determine the type fields of arguments to use
  // according to function's definition.
  virtual Status ForwardCompute(const std::vector<const Argument *> &args,
                                const std::vector<Argument *> *activations,
                                const FunctionDef &def) = 0;

  // @grad is this function's gradient. gradient * f'(x) will be the former
  // function's gradient
  // @x is this function's forword input.
  // @previous_grad is previous functions gradient.
  // previous_grad += f'(x) * grad
  virtual Status
  BackwardCompute(const std::vector<const Argument *>& x, const Argument &grad,
                  const std::vector<Argument *> *previous_grad) = 0;
};

// Base class for all funcs
class Function : public FunctionInterface {
public:
  // void SetExecContext(ExecContext *context);
  Function() {}

  virtual void CheckContext() = 0;

  // Function registerer must use signature as key.
  static std::string Signature(const std::string &name, DataType dtype) {
    return strings::Printf("%s:%d", name.c_str(), dtype);
  }

  // Parse signature and extract different infomation fields.
  // Return true if successfully parsed, else false.
  static bool ParseSignature(const std::string &sign, std::string *name,
                             DataType *dtype) {
    auto pieces = strings::Split(sign, ':');
    if (pieces.size() != 2)
      return false;
    *name = pieces[0];
    *dtype = static_cast<DataType>(std::stoi(pieces[1]));
    return true;
  }

  // Create an func from definition
  // @def: func's definition
  // @attrs: node's attrs
  // The func's attributes is registered in `attrs`, this api should read
  // definition's field names and try to get value from `attrs`.
  virtual Status
  FromDef(const FunctionDef &def,
          const ::google::protobuf::Map<::std::string, ::chasky::AttrValue>
              &attrs) = 0;

  StringPiece Name() const { return name_; }

  const FunctionDef &Def() const { return def_; }

  // const ExecContext &exec_context() { return *exec_context_; }
  // ExecContext *mutable_exec_context() { return exec_context_; }

private:
  // ExecContext *exec_context_;
  StringPiece name_;
  FunctionDef def_;
};

class FunctionLibrary {
public:
  typedef std::function<std::unique_ptr<Function>()> FunctionCreatorType;

  // Singleton, the global func library
  static FunctionLibrary &Instance() {
    static FunctionLibrary *library = new FunctionLibrary();
    return *library;
  }

  // Register an func creator with a name, if another func called the
  // same name exists, then it will not insert the new func and just
  // return error.
  // NOTE It is not thread-safe.
  Status Register(const std::string &name,
                  FunctionCreatorType &&creator) CH_USE_RESULT;

  // Look up the library for the func which match the name, if not exits,
  // just return error.
  // NOTE It is not Rewrie-Read thread-sefe.
  Status LookUp(const std::string &name,
                FunctionCreatorType **creator) CH_USE_RESULT;

  // Human readable description of all the func creator store in
  // the library.
  std::string DebugString() const;

  // Number of func creators in the library
  size_t Size() const;

private:
  std::unordered_map<std::string, FunctionCreatorType> op_library_;
};

// #define REGISTER_OP(NAME, CLASS)                                               \
//   FunctionLibrary::Instance().Register(                                        \
//       NAME, [] { return std::unique_ptr<Function>(new CLASS); });
}

#endif
