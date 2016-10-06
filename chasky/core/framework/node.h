#ifndef CHASKY_CORE_FRAMEWORK_NODE_H_
#define CHASKY_CORE_FRAMEWORK_NODE_H_
#include <memory>
#include <thread>

#include "chasky/core/runtime/postbox.h"
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/function.h"
#include "chasky/core/framework/argument.h"
#include "chasky/core/runtime/edge_lib.h"

namespace chasky {

class Node;

using node_ptr_t = std::shared_ptr<Node>;

// enum class TaskType { FORWARD, BACKWARD };

// Node is a node of compute graph. It is the carrier of the corresponding
// function.
// Node starts a thread as the function's service. The Service will receive
// Edges' tasks and start a ForwardCompute or BackwardCompute task.
// After Node's task is finished, the output arguments will be released to the
// out-link edges, and other nodes will receive this arguments.
class Node {
public:
  typedef std::function<std::string(const std::string &node_name,
                                    const std::string &arg_name)> key_creator_t;
  // Create a node from definition
  static std::unique_ptr<Node> Create(const NodeDef &def, PostBox *postbox,
                                      EdgeLib *edge_lib);

  Node(Node &&other);

  Status Compute(TaskType task_type);

  StringPiece Name() const { return def_.name(); }

  NodeDef &Definition() { return def_; }

  // ~Node();

protected:
  Status ForwardCompute();

  Status BackwardCompute();

  // Collect input arguments from postbox.
  Status CollectInputArguments();
  // Collect output arguments from postbox.
  Status CollectOutputArguments();
  // Collect input's gradient arguments.
  Status CollectInputGradArguments();
  // Collect gradient of this node and source nodes which point to this.
  Status CollectOutputGradArguments();

  // @direction:
  //    0 : direct
  //    1 : forward
  //    -1 : backward
  Status CollectArguments(
      std::vector<ArgumentPtr> *args,
      const google::protobuf::RepeatedPtrField<ArgumentDef> &arg_defs,
      std::vector<string> *arg_keys, int direction, key_creator_t key_creator,
      bool force = false);

  Status CollectParameters();

  // Tell others that this function's activation is ready.
  Status ReleaseActivations();
  // Tell others that this function's graidents is ready.
  Status ReleaseGradients();

  // Register forward activation and backward gradient into postbox. The forward
  // activations are registered by key like format "{node_name}:{arg_name}", the
  // backward arguments' keys are like "{node_name}:{arg_name}_backward"
  // NOTE this method should be called before ForwardCompute and BackwardCompute
  Status RegisterArguments();

  // Register model parameters to global library.
  Status RegisterParameters();

  Node(const NodeDef &def, PostBox *postbox, EdgeLib *edge_lib);

private:
  NodeDef def_;
  FunctionDef *func_def_;
  PostBox *postbox_;
  EdgeLib *edge_lib_;
  // Current task
  TaskType cur_task_;
  // the corresponding function
  std::unique_ptr<Function> func_;
  std::vector<ArgumentPtr> params_;
  std::vector<string> input_grads_keys_;
  std::vector<string> output_grads_keys_;
  std::vector<string> input_keys_;
  std::vector<string> output_keys_;
};

} // namespace chasky
#endif
