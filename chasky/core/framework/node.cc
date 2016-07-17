#include "chasky/common/macros.h"
#include "chasky/common/strings.h"
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/function_def_builder.h"

namespace chasky {
using std::string;

std::string GenEdgeKey(const string &node_name, const string &target_name,
                       const string &arg_name) {
  return strings::Printf("%s:%s->%s", node_name.c_str(), arg_name.c_str(),
                         target_name.c_str());
}

std::string GenEdgeKey(const std::string &input, const std::string &node2) {
  return strings::Printf("%s->%s", input.c_str(), node2.c_str());
}

Edge::Edge(const Node *src, const Node *trg, const std::string &arg)
    : signature_(
          GenEdgeKey(src->Name().tostring(), trg->Name().tostring(), arg)),
      src_(src), trg_(trg) {}

Node::Node(const NodeDef &def) : def_(def), func_def_(nullptr) {
  CHECK(!def_.name().empty());
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
  CHECK(!def_.def_name().empty()) << "should extract def_name from signature first";
  CHECK_EQ(def_.def_name(), func_def_->name());

  CH_CHECK_OK(
      FunctionLibrary::Instance().LookUp(def_.signature(), &func_creator));
  // init function's definition by filling attributes from node's definition.
  // TODO much code here
  // TODO just add unittest
  CH_CHECK_OK(
      FunctionDefLibrary::Instance().LookUp(def_.def_name(), &func_def_));

  DLOG(INFO) << "creating function " << func_def_->name();
  // create an function
  func_ = (*func_creator)();
  DLOG(INFO) << "func is created";
  CH_CHECK_OK(func_->FromDef(*func_def_, def_.attr()));

  CreateOutputArguments();
  CreateModelParameters();
}

std::unique_ptr<Node> Node::Create(const NodeDef &def) {
  std::unique_ptr<Node> node(new Node(def));
  return node;
}

Status Node::Compute(chasky::TaskType task) {
  if (task == FORWORD) {
    ForwardCompute();
  } else {
    BackwardCompute();
  }
  return Status();
}

Status Node::ForwardCompute() {
  // Prepare inputs
  std::vector<const Argument *> inputs;
  std::vector<Argument *> outputs;
  for (const auto &edge : inlinks_) {
    inputs.push_back(&edge->Activation());
  }
  for (auto &output : outputs_) {
    outputs.push_back(&output);
  }

  Status status = func_->ForwardCompute(inputs, &outputs, *func_def_);
  return status;
}

Status Node::BackwardCompute() {
  // TODO
  UN_IMPLEMENTED
  return Status();
}

Status Node::ReleaseEdge(const Edge *edge) {
  UN_IMPLEMENTED
  return Status();
}

void Node::CreateOutputArguments() {
  FunctionDef *func_def;
  CH_CHECK_OK(FunctionDefLibrary::Instance().LookUp(def_.def_name(), &func_def));

  for (size_t i = 0; i < func_def->outputs_size(); i++) {
    const auto &output_def = func_def->outputs(i);
    DLOG(INFO) << "output_def:\t" << &output_def;
    outputs_.emplace_back(&output_def);
  }
}

void Node::CreateModelParameters() { UN_IMPLEMENTED }

Status Node::StartService() {
  UN_IMPLEMENTED
  return Status();
}

Status Node::ConnectTo(Node *other, bool forward_or_bachward) {
  auto status = Status();
  Node *src = this;
  Node *trg = other;

  if (!forward_or_bachward) {
    src = other;
    trg = this;
  }

  return status;
}

Node::~Node() {
  if (service_thread_.joinable()) {
    service_thread_.join();
  }
}

} // namespace chasky
