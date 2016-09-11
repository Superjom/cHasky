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

enum class TaskType { FORWARD, BACKWARD };

// Node is a node of compute graph. It is the carrier of the corresponding
// function.
// Node starts a thread as the function's service. The Service will receive
// Edges' tasks and start a ForwardCompute or BackwardCompute task.
// After Node's task is finished, the output arguments will be released to the
// out-link edges, and other nodes will receive this arguments.
class Node {
public:
  // Create a node from definition
  static std::unique_ptr<Node> Create(const NodeDef &def, PostBox *postbox,
                                      EdgeLib *edge_lib);

  Node(Node &&other);

  Status Compute(TaskType task_type);

  StringPiece Name() const { return def_.name(); }

  // Start the running threads.
  Status StartService();

  ~Node();

  void ServiceThreadJoin() {
    if (service_thread_ && service_thread_->joinable()) {
      service_thread_->join();
    }
  }

protected:
  Status ForwardCompute();

  Status BackwardCompute();

  // Input arguments.
  Status CollectInArgItems();
  // Gradient arguments.
  Status CollectOutArgItems();

  Status CollectParameters();

  // Tell others that this function's activation is ready.
  Status ReleaseActivations();
  // Tell others that this function's graidents is ready.
  Status ReleaseGradients();

  // Status PrepareInArguments();

  // Status PrepareOutArguments();

  // Register forward activation and backward gradient into postbox.
  Status RegisterArguments();

  // Register model parameters to global library.
  Status RegisterParameters();

  Node(const NodeDef &def, PostBox *postbox, EdgeLib *edge_lib);

  std::thread &ServiceThread() const {
    CHECK(service_thread_.get()) << "service is not started";
    return *service_thread_;
  }

private:
  NodeDef def_;
  FunctionDef *func_def_;
  // Current task
  TaskType cur_task_;
  // the corresponding function
  std::unique_ptr<Function> func_;

  std::vector<PostBox::ArgItem *> in_arg_items_;
  std::vector<PostBox::ArgItem *> out_arg_items_;

  // argument cache for Function
  std::vector<const Argument *> inputs_;
  std::vector<Argument *> outputs_;
  std::vector<ArgumentPtr> params_;

  std::vector<ArgumentPtr> out_args_;

  std::condition_variable in_args_ready_;
  std::condition_variable out_args_ready_;
  std::mutex cond_lock_;

  PostBox *postbox_;
  EdgeLib *edge_lib_;

  std::unique_ptr<std::thread> service_thread_;
};

} // namespace chasky
#endif
