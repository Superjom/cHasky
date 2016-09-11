#include "chasky/common/macros.h"
#include "chasky/common/strings.h"
#include "chasky/core/framework/node.h"
#include "chasky/core/runtime/parameter_lib.h"
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

Status Node::Compute(TaskType task_type) {
  if (task_type == TaskType::FORWARD) {
    return ForwardCompute();
  }
  return BackwardCompute();
}

Status Node::StartService() {
  Status status;

  DLOG(INFO) << "Node " << def_.name() << " start service";

  auto task = [&, this] {
    while (RuntimeState::Instance().KeepRunning()) {
      auto forward_status = Compute(TaskType::FORWARD);
      status.Update(forward_status);
      RuntimeState::Instance().UpdateStatus(status);
      if (!RuntimeState::Instance().KeepRunning())
        break;

      // auto backward_status = Compute(TaskType::BACKWARD);
      // status.Update(backward_status);
      // RuntimeState::Instance().UpdateStatus(status);
      // if (RuntimeState::Instance().KeepRunning())
      //   break;
    }
    LOG(INFO) << "Node " << def_.name() << " quit service!";
  };

  CHECK(!service_thread_.get()) << "duplicate start node's service";
  service_thread_.reset(new std::thread(task));

  return status;
}

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
  CHECK(postbox_);

  auto callback = [&](Argument *arg) {
    inputs_.push_back(arg);
    if (++num_in_args_ready == func_def_->inputs_size()) {
      std::unique_lock<std::mutex> lock(cond_lock_);
      in_args_ready_.notify_one();
    }
  };

  for (const auto &arg_def : func_def_->inputs()) {
    // find source of the argument in need.
    const auto &arg_name = arg_def.name();
    auto arg_key = EdgeLib::CreateArgKey(node_name, arg_name);
    std::string source_arg_key;
    CH_CHECK_OK(edge_lib_->RetriveByTarget(arg_key, &source_arg_key));

    postbox_->Consume(source_arg_key, callback);

    std::unique_lock<std::mutex> lock(cond_lock_);
    in_args_ready_.wait(
        lock, [&] { return num_in_args_ready == func_def_->inputs_size(); });
    DLOG(INFO) << "received Argument " << source_arg_key << " "
               << inputs_.back();
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

Status Node::CollectParameters() {
  Status status;
  params_.clear();
  const auto &node_name = def_.name();
  for (auto &arg_def : func_def_->params()) {
    auto arg_key = PostBox::CreateArgKey(node_name, arg_def.name());
    params_.emplace_back(nullptr);
    CH_CHECK_OK(ParameterLib::Instance().Retrieve(arg_key, &params_.back()));
  }
  return status;
}

Status Node::ReleaseActivations() {
  DLOG(INFO) << "release activations ...";
  Status status;
  const auto &node_name = def_.name();

  for (size_t i = 0; i < func_def_->inputs_size(); i++) {
    const auto &arg_def = func_def_->outputs(i);
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
  for (size_t i = 0; i < func_def_->outputs_size(); i++) {
    const auto &arg_def = func_def_->outputs(i);
    const auto &arg_name = arg_def.name();
    auto arg_key = EdgeLib::CreateArgKey(node_name, arg_name);
    DLOG(INFO) << "register Argument " << arg_key << " to postbox";
    // postbox_->Register(arg_key);
    out_args_.emplace_back(std::make_shared<Argument>(&arg_def));
    DLOG(INFO) << "Node create output argument "
               << out_args_.back()->Description();

    DLOG(INFO) << "input arg " << i << " " << arg_def.DebugString();

    CHECK_EQ(arg_def.shape().width(), out_args_.back()->Shape().width());
    CHECK_EQ(arg_def.shape().height(), out_args_.back()->Shape().height());
    CHECK_GT(arg_def.shape().height(), 0UL);
    CHECK_GT(arg_def.shape().width(), 0UL);

    // CHECK(out_args_.back()->ArgField()->float_mat_val);
    // register resource to postbox
    CH_CHECK_OK(postbox_->Register(arg_key, out_args_.back()));
  }

  return status;
}

Status Node::RegisterParameters() {
  Status status;
  const auto &node_name = def_.name();
  for (auto &arg_def : func_def_->params()) {
    auto arg_key = PostBox::CreateArgKey(node_name, arg_def.name());
    CH_CHECK_OK(ParameterLib::Instance().RegisterCreate(arg_key, arg_def));
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
  CH_CHECK_OK(func_->FromDef(*func_def_, def_.attr()));

  CH_CHECK_OK(RegisterArguments());

  CH_CHECK_OK(RegisterParameters());
}

Node::~Node() {
  if (service_thread_ && service_thread_->joinable()) {
    service_thread_->join();
  }
}
}
