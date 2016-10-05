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
  typedef ::google::protobuf::Map<::std::string, ::chasky::AttrValue>
      extra_attr_t;
  // A Node will determine which function to use. And a function will init
  // itself according to function's definition(attributes).
  virtual Status InitFromProto(const FunctionDef &def) = 0;

  virtual Status ForwardCompute() = 0;

  virtual Status BackwardCompute() = 0;

  virtual const FunctionDef &Def() { return def_; }

private:
  FunctionDef def_;
};

// Base class for all funcs
class Function : public FunctionInterface {
public:
  // A data structure that contains every parameter that forward/backward
  // computation needs. Just make computation's interface more simple.
  struct ComputationItem {
    std::vector<ArgumentPtr> inputs;
    std::vector<ArgumentPtr> input_grads;
    std::vector<ArgumentPtr> outputs;
    std::vector<ArgumentPtr> output_grads;
    // weight's grad to update weight parameter.
    std::vector<ArgumentPtr> weight_grads;
  };

  Function() {}

  // Set model's parameters before function's compuatation.
  void SetModelParameters(std::vector<ArgumentPtr> *params) {
    params_ = params;
  }

  ComputationItem &CompItem() { return comp_item_; }

  virtual void CheckContext() = 0;

  // Function registerer must use signature as key.
  static std::string Signature(const std::string &name, DataType dtype) {
    return strings::Printf("%s:%d", name.c_str(), dtype);
  }

  // Parse signature and extract different infomation fields.
  // Return true if successfully parsed, else false.
  static bool ParseSignature(const std::string &sign, std::string *name,
                             DataType *dtype);

  // Create an func from definition
  // @def: func's definition
  // @attrs: node's attrs
  // The func's attributes is registered in `attrs`, this api should read
  // definition's field names and try to get value from `attrs`.
  virtual Status FromDef(FunctionDef &def, const extra_attr_t &attrs) = 0;

  StringPiece Name() const { return name_; }

  ArgumentPtr Input(size_t i) {
    CHECK_LT(i, comp_item_.inputs.size());
    return comp_item_.inputs[i];
  }

  ArgumentPtr Output(size_t i) {
    CHECK_LT(i, comp_item_.outputs.size());
    return comp_item_.outputs[i];
  }

  ArgumentPtr InputGrad(size_t i) {
    CHECK_LT(i, InputGrads().size());
    return InputGrads()[i];
  }

  ArgumentPtr OutputGrad(size_t i) {
    CHECK_LT(i, OutputGrads().size());
    return OutputGrads()[i];
  }

  ArgumentPtr &WeightGrad(size_t i) {
    CHECK_LT(i, WeightGrads().size());
    return WeightGrads()[i];
  }

  std::vector<ArgumentPtr> &Inputs() { return comp_item_.inputs; }

  std::vector<ArgumentPtr> &Outputs() { return comp_item_.outputs; }

  std::vector<ArgumentPtr> &InputGrads() { return comp_item_.input_grads; }

  std::vector<ArgumentPtr> &OutputGrads() { return comp_item_.output_grads; }

  std::vector<ArgumentPtr> &WeightGrads() { return comp_item_.weight_grads; }

protected:
  FunctionDef *def_;
  // model parameters
  std::vector<ArgumentPtr> *params_;
  ComputationItem comp_item_;

private:
  StringPiece name_;
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
