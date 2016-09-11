#include "chasky/common/macros.h"
#include "chasky/common/strings.h"
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/function_def_builder.h"
#include "chasky/core/runtime/state.h"

namespace chasky {

std::unique_ptr<Node> Node::Create(const NodeDef &def, PostBox *postbox,
                                   EdgeLib *edge_lib) {
  return std::unique_ptr<Node>{new Node(def, postbox, edge_lib)};
}

Node::Node(Node &&other)
    : def_(other.def_), func_def_(other.func_def_), cur_task_(other.cur_task_),
      func_(std::move(other.func_)) {}

Status Node::ForwardCompute() {
  Status status;
  CH_CHECK_OK(CollectInArgItems());
  CH_CHECK_OK(CollectOutArgItems());

  CH_CHECK_OK(func_->ForwardCompute(inputs_, &out_args_));

  CH_CHECK_OK(ReleaseActivations());

  return status;
}

Status Node::BackwardCompute() { UN_IMPLEMENTED }

Status Node::CollectInArgItems() {
  Status status;
  inputs_.clear();
  const auto &node_name = def_.name();
  int num_in_args_ready = 0;
  for (const auto &arg_def : func_def_->inputs()) {
    const auto &arg_name = arg_def.name();
    auto arg_key = EdgeLib::CreateArgKey(node_name, arg_name);
    auto callback = [&](Argument *arg) {
      num_in_args_ready++;
      inputs_.push_back(arg);
      if (num_in_args_ready == func_def_->inputs_size()) {
        in_args_ready_.notify_one();
      }
    };
    postbox_->Consume(arg_key, callback);
  }
  return status;
}

Status Node::CollectOutArgItems() {
  Status status;
  outputs_.clear();
  for (auto &arg : out_args_) {
    CHECK(arg);
    outputs_.push_back(arg.get());
  }
  return status;
}

Status Node::ReleaseActivations() {
  Status status;
  const auto &node_name = def_.name();

  for (size_t i = 0; i < func_def_->inputs_size(); i++) {
    const auto &arg_def = func_def_->inputs(i);
    const auto &arg_name = arg_def.name();
    auto arg_key = EdgeLib::CreateArgKey(node_name, arg_name);
    postbox_->Send(arg_key, out_args_[i]);
  }
  return status;
}

Status Node::RegisterArguments() {
  Status status;
  const auto &node_name = def_.name();
  out_args_.clear();
  for (size_t i = 0; i < func_def_->inputs_size(); i++) {
    const auto &arg_def = func_def_->inputs(i);
    const auto &arg_name = arg_def.name();
    auto arg_key = EdgeLib::CreateArgKey(node_name, arg_name);
    DLOG(INFO) << "register Argument " << arg_key << " to postbox";
    // postbox_->Register(arg_key);
    out_args_.emplace_back(std::make_shared<Argument>(&arg_def));
    DLOG(INFO) << "Node create output argument "
               << out_args_.back()->Description();
  }

  return status;
}

Node::Node(const NodeDef &def, PostBox *postbox, EdgeLib *edge_lib)
    : def_(def), func_def_(nullptr), postbox_(postbox), edge_lib_(edge_lib) {
  CHECK(!def_.name().empty());
  // CHECK(graph_ != nullptr);
  DLOG(INFO) << "creating node " << def_.name();
  // extract information from signature
  std::string def_name;
  DataType dtype;
  CHECK(Function::ParseSignature(def_.signature(), &def_name, &dtype));
  def_.set_def_name(def_name);
  def_.set_dtype(dtype);

  Status status;
  FunctionLibrary::FunctionCreatorType *func_creator;
  CH_CHECK_OK(
      FunctionDefLibrary::Instance().LookUp(def_.def_name(), &func_def_));
  CHECK(func_def_ != nullptr);
  CHECK(!def_.def_name().empty())
      << "should extract def_name from signature first";
  CHECK_EQ(def_.def_name(), func_def_->name());

  CH_CHECK_OK(
      FunctionLibrary::Instance().LookUp(def_.signature(), &func_creator));
  // init function's definition by filling attributes from node's
  // definition.
  // TODO much code here
  // TODO just add unittest
  CH_CHECK_OK(
      FunctionDefLibrary::Instance().LookUp(def_.def_name(), &func_def_));

  DLOG(INFO) << "creating function " << func_def_->name();
  // create an function
  func_ = (*func_creator)();
  DLOG(INFO) << "func is created";
  CH_CHECK_OK(func_->FromDef(*func_def_, def_.attr()));
}
}
