#include "chasky/common/macros.h"
#include "chasky/common/strings.h"
#include "chasky/core/framework/node.h"
#include "chasky/core/runtime/parameter_lib.h"
#include "chasky/core/framework/function_def_builder.h"
#include "chasky/core/runtime/state.h"

namespace chasky {
using namespace std;

// namespace {
// class ArgumentsCollector {
// public:
//   typedef std::function<std::string(const std::string &node_name,
//                                     const std::string &arg_name)>
//                                     key_creator_t;

//   ArgumentsCollector(PostBox *postbox, EdgeLib *edge_lib,
//                      const std::string &node_name)
//       : postbox_(postbox), edge_lib_(edge_lib), node_name_(node_name) {}

//   const std::vector<std::string> *
//   CollectArgumentKeys(const std::string &arg_name, int direction,
//                       key_creator_t key_creator) {
//     auto arg_key = key_creator(node_name_, arg_name);
//     const std::vector<std::string> *nodes;
//     std::vector<std::string> single_node;
//     switch (direction) {
//     case 1:
//       CH_CHECK_OK(edge_lib_->RetriveByTarget(arg_key, &nodes));
//       break;
//     case -1:
//       CH_CHECK_OK(edge_lib_->RetriveBySource(arg_key, &nodes));
//       break;
//     case 0: {
//       single_node.push_back(arg_key);
//       nodes = &single_node;
//     } break;
//     default:
//       LOG(FATAL) << "not supported direction";
//     }
//     return nodes;
//   }

//   std::vector<ArgumentPtr> GatherSingleArgument(const vector<string> &nodes,
//                                                 int direction, bool force) {
//     ArgumentPtr arg;
//     std::vector<ArgumentPtr> args;
//     CHECK_EQ(nodes.size(), 1UL);
//     if (postbox_->Consume(nodes.front(), &arg, force).ok() && arg) {
//       args.emplace_back(arg);
//     } else if (direction == 0) {
//       if (postbox_->Consume(nodes.front(), &arg, force).ok() && arg) {
//         args.emplace_back(arg);
//       }
//     }
//     return args;
//   }

//   std::vector<ArgumentPtr> GatherListArgument(const vector<string> &nodes,
//                                               int direction, bool force) {
//     std::vector<ArgumentPtr> args;
//     std::vector<ArgumentPtr> field_args;
//     for (auto &src_arg : nodes) {
//       ArgumentPtr arg;
//       if (postbox_->Consume(src_arg, &arg, force).ok()) {
//         // data provider's backward argument is nullptr, ignore this case
//         if (arg)
//           field_args.emplace_back(arg);
//       } else {
//         return args;
//       }
//     }

//     ArgumentPtr list_arg = std::make_shared<Argument>();
//     CH_CHECK_OK(list_arg->SetList(field_args));
//     args.emplace_back(list_arg);
//     return args;
//   }

//   std::vector<ArgumentPtr> CollectInputArguments() {
//     key_creator_t key_creator = [](const std::string &node_name,
//                                    const std::string &arg_name) {
//       return PostBox::CreateArgKey(node_name, arg_name, FORWARD);
//     };
//     auto arg_keys = CollectArgumentKeys()

//   }

// private:
//   PostBox *postbox_;
//   EdgeLib *edge_lib_;
//   std::string node_name_;
//   std::vector<std::string> arg_keys_;
//   // cache the argument keys(in postbox) to simplify collection period.
//   std::vector<std::string> input_arg_keys_;
//   std::vector<std::string> output_arg_keys_;
//   std::vector<std::string> input_grad_arg_keys_;
//   std::vector<std::string> output_grad_arg_keys_;
// };
// }

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

Status Node::ForwardCompute() {
  Status status;
  CH_TEST_OR_RETURN(CollectInputArguments());
  CH_TEST_OR_RETURN(CollectOutputArguments());
  DLOG(INFO) << "begin to forward compute";
  CH_CHECK_OK(func_->ForwardCompute());
  CH_CHECK_OK(ReleaseActivations());
  return status;
}

Status Node::BackwardCompute() {
  Status status;
  // Only normal node have output gradient.
  if (func_def_->func_type() == FunctionDef_FuncType_NORMAL) {
    CH_TEST_OR_RETURN(CollectOutputGradArguments());
  }
  CH_TEST_OR_RETURN(CollectInputGradArguments());
  CH_CHECK_OK(func_->BackwardCompute());
  CH_CHECK_OK(ReleaseGradients());
  return status;
}

Status Node::CollectInputArguments() {
  DLOG(INFO) << "collecting input arguments ..";
  auto &args = func_->CompItem().inputs;
  const auto &arg_defs = func_def_->inputs();
  key_creator_t key_creator = [](const std::string &node_name,
                                 const std::string &arg_name) {
    return PostBox::CreateArgKey(node_name, arg_name, FORWARD);
  };
  return CollectArguments(&args, arg_defs, &input_keys_, 1, key_creator);
}

Status Node::CollectOutputArguments() {
  DLOG(INFO) << "collecting output arguments ..";
  auto &args = func_->CompItem().outputs;
  const auto &arg_defs = func_def_->outputs();
  key_creator_t key_creator = [](const std::string &node_name,
                                 const std::string &arg_name) {
    return PostBox::CreateArgKey(node_name, arg_name, FORWARD);
  };
  return CollectArguments(&args, arg_defs, &output_keys_, 0, key_creator,
                          true /*force*/);
}

Status Node::CollectInputGradArguments() {
  DLOG(INFO) << "collecting input grad arguments ..";
  Status status;
  auto &args = func_->CompItem().input_grads;
  const auto &arg_defs = func_def_->inputs();
  key_creator_t key_creator = [](const std::string &node_name,
                                 const std::string &arg_name) {
    return PostBox::CreateArgKey(node_name, arg_name, BACKWARD);
  };
  return CollectArguments(&args, arg_defs, &input_grads_keys_, -1, key_creator,
                          true /*force*/);
}

Status Node::CollectOutputGradArguments() {
  Status status;
  // loss function do not need output gradient.
  if (func_def_->func_type() == FunctionDef_FuncType_LOSS)
    return status;

  auto &args = func_->CompItem().output_grads;
  const auto &arg_defs = func_def_->outputs();
  key_creator_t key_creator = [](const std::string &node_name,
                                 const std::string &arg_name) {
    return PostBox::CreateArgKey(node_name, arg_name, BACKWARD);
  };
  return CollectArguments(&args, arg_defs, &output_grads_keys_, 0, key_creator,
                          false /*force*/);
}

Status Node::CollectArguments(
    std::vector<ArgumentPtr> *args,
    const google::protobuf::RepeatedPtrField<ArgumentDef> &arg_defs,
    std::vector<string> *arg_keys, int direction, key_creator_t key_creator,
    bool force) {
  Status status;
  args->clear();
  arg_keys->clear();
  const auto &node_name = def_.name();

  for (const auto &arg_def : arg_defs) {
    // find source of the argument in need.
    const auto &arg_name = arg_def.name();
    auto arg_key = key_creator(node_name, arg_name);
    const std::vector<std::string> *nodes;
    std::vector<std::string> single_node;
    switch (direction) {
    case 1:
      CH_CHECK_OK(edge_lib_->RetriveByTarget(arg_key, &nodes));
      break;
    case -1:
      CH_CHECK_OK(edge_lib_->RetriveBySource(arg_key, &nodes));
      break;
    case 0: {
      single_node.push_back(arg_key);
      nodes = &single_node;
    } break;
    default:
      LOG(FATAL) << "not supported direction";
    }
    // Argument with only one data field
    if (nodes->size() == 1UL) {
      ArgumentPtr arg;
      if (postbox_->Consume(nodes->front(), &arg, force).ok()) {
        if (arg) {
          DLOG(INFO) << Name() << " collecting " << nodes->front()
                     << " successfully";
          arg_keys->push_back(nodes->front());
          args->emplace_back(arg);
        }
      } else if (direction == 0) {
        CH_TEST_OR_RETURN(postbox_->Consume(nodes->front(), &arg, force));
        if (arg) {
          arg_keys->push_back(nodes->front());
          args->emplace_back(arg);
        } else {
          return Status(error::NOT_INITED, "");
        }
      } else { // Argument with list data fields
        // DLOG(INFO) << ">> " << Name() << "collecting list data fields";
        std::vector<ArgumentPtr> field_args;
        for (auto &src_arg : *nodes) {
          ArgumentPtr arg;
          if (postbox_->Consume(src_arg, &arg, force).ok()) {
            // data provider's backward argument is nullptr, ignore this case
            if (arg) {
              arg_keys->push_back(src_arg);
              field_args.emplace_back(arg);
            }
          } else {
            return Status(error::NOT_INITED, "");
          }
        }

        ArgumentPtr list_arg = std::make_shared<Argument>();
        CH_CHECK_OK(list_arg->SetList(field_args));
        args->emplace_back(list_arg);
      }
    }
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

  for (size_t i = 0; i < func_def_->outputs_size(); i++) {
    const auto &arg_def = func_def_->outputs(i);
    const auto &arg_name = arg_def.name();
    auto arg_key = PostBox::CreateArgKey(node_name, arg_name, FORWARD);
    CHECK(func_->CompItem().outputs[i]);
    postbox_->Send(arg_key, func_->CompItem().outputs[i]);
  }
  return status;
}

Status Node::ReleaseGradients() {
  Status status;
  // const auto &node_name = def_.name();
  // for (size_t i = 0; i < func_def_->inputs_size(); i++) {
  //   const auto &arg_def = func_def_->inputs(i);
  DLOG(INFO) << "release gradients";
  CHECK_EQ(func_->CompItem().input_grads.size(), input_grads_keys_.size());
  for (size_t i = 0; i < input_grads_keys_.size(); i++) {
    // for (const auto &arg_key : input_grads_keys_) {
    // const auto &arg_name = arg_def.name();
    // auto arg_key = PostBox::CreateArgKey(node_name, arg_name, BACKWARD);
    // CHECK(func_->CompItem().input_grads[i]);
    auto &arg_key = input_grads_keys_[i];
    DLOG(INFO) << "release gradient to " << arg_key;
    postbox_->Send(arg_key, func_->CompItem().input_grads[i]);
  }
  return status;
};

Status Node::RegisterArguments() {
  Status status;
  const auto &node_name = def_.name();
  for (size_t i = 0; i < func_def_->outputs_size(); i++) {
    const auto &arg_def = func_def_->outputs(i);
    const auto &arg_name = arg_def.name();
    auto forward_arg_key = PostBox::CreateArgKey(node_name, arg_name, FORWARD);
    DLOG(INFO) << "register forward Argument " << forward_arg_key
               << " to postbox";
    auto arg = std::make_shared<Argument>(&arg_def);
    DLOG(INFO) << "Node create output argument " << arg->Description();
    DLOG(INFO) << "input arg " << i << " " << arg_def.DebugString();
    CHECK_EQ(arg_def.shape().width(), arg->Shape().width());
    CHECK_GT(arg_def.shape().width(), 0UL);
    CH_CHECK_OK(postbox_->Register(forward_arg_key, arg, true));

    auto backward_arg_key =
        PostBox::CreateArgKey(node_name, arg_name, BACKWARD);
    ArgType arg_type = ArgType::NORMAL;
    if (func_def_->func_type() == FunctionDef_FuncType_LOSS) {
      arg_type = ArgType::PLACE_HOLDER;
      arg = nullptr;
    } else {
      arg = std::make_shared<Argument>(&arg_def);
    }
    DLOG(INFO) << "register backward Argument " << backward_arg_key
               << " to postbox";
    CH_CHECK_OK(postbox_->Register(backward_arg_key, arg, false, arg_type));
  }

  return status;
}

Status Node::RegisterParameters() {
  Status status;
  params_.clear();
  const auto &node_name = def_.name();
  for (auto &arg_def : func_def_->params()) {
    auto arg_key = PostBox::CreateArgKey(node_name, arg_def.name());
    ArgumentPtr ptr;
    CH_CHECK_OK(
        ParameterLib::Instance().RegisterCreate(arg_key, arg_def, &ptr));
    params_.emplace_back(std::move(ptr));
  }
  return status;
}

Node::Node(const NodeDef &def, PostBox *postbox, EdgeLib *edge_lib)
    : def_(def), func_def_(nullptr), postbox_(postbox), edge_lib_(edge_lib) {
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
  CHECK(!def_.def_name().empty())
      << "should extract def_name from signature first";
  CHECK_EQ(def_.def_name(), func_def_->name());

  CH_CHECK_OK(
      FunctionLibrary::Instance().LookUp(def_.signature(), &func_creator));
  CH_CHECK_OK(
      FunctionDefLibrary::Instance().LookUp(def_.def_name(), &func_def_));

  DLOG(INFO) << "creating function " << func_def_->name();
  // create an function
  func_ = (*func_creator)();
  CH_CHECK_OK(func_->FromDef(*func_def_, def_.attr()));
  CH_CHECK_OK(RegisterArguments());
  CH_CHECK_OK(RegisterParameters());
  func_->SetModelParameters(&params_);
}
}
