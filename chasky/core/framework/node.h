#ifndef CHASKY_CORE_FRAMEWORK_NODE_H_
#define CHASKY_CORE_FRAMEWORK_NODE_H_
#include <memory>
#include <thread>
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/function.h"
#include "chasky/core/framework/argument.h"
#include "chasky/core/framework/edge.h"
namespace chasky {

class Node;
class Graph;

using node_ptr_t = std::shared_ptr<Node>;

// Node is a node of compute graph. It is the carrier of the corresponding
// function.
// Node starts a thread as the function's service. The Service will receive
// Edges' tasks and start a ForwardCompute or BackwardCompute task.
// After Node's task is finished, the output arguments will be released to the
// out-link edges, and other nodes will receive this arguments.
class Node {
public:
  // Create a node from definition
  static std::unique_ptr<Node> Create(const NodeDef &def, Graph *graph);

  struct OutputArgument {
    ArgumentPtr activation;
    ArgumentPtr grad;

    // TODO how to init gradient?
    OutputArgument(const ArgumentDef* def) :
      activation(std::make_shared<Argument>(def)) {}
  };

  Node(Node &&other)
      : def_(std::move(other.def_)), func_def_(other.func_def_),
        cur_task_(other.cur_task_), func_(std::move(other.func_)),
        grad_(std::move(other.grad_)),
        service_thread_(std::move(other.service_thread_)),
        graph_(other.graph_) {}

  // Call ForwardCompute or BackwardCompute
  Status Compute(TaskType task);
  // Forward computation.
  Status ForwardCompute();
  // Backward computation.
  Status BackwardCompute();
  // Start a thread to offer function's computation service.
  Status StartService();

  const Function &GetFunction() const;
  // const ExecContext &GetExecContext() const;

  Function *mutable_function();
  // ExecContext *mutable_exec_context();

  // Connect this node to another node.
  // @other: the target node.
  // @forward_or_backward: connect other nodes as forward or backward end.
  Status ConnectTo(Node *other, bool forward_or_bachward);

  StringPiece Name() const { return def_.name(); }

  ~Node();

protected:
  // Tell the edge's endpoints that the argument is ready.
  Status ReleaseEdge(const std::string &edge);
  Status ReleaseEdge(const Edge *edge);
  void CreateOutputArguments();
  void CreateModelParameters();

  // Create a node, including create the corresponding function's object.
  Node(const NodeDef &def, Graph *graph);

private:
  NodeDef def_;
  FunctionDef *func_def_;
  // Current task
  TaskType cur_task_;
  // the corresponding function
  std::unique_ptr<Function> func_;

  // each input is an input-node's output-argument
  std::vector<ArgumentPtr> inputs_;
  // output arguments
  std::vector<OutputArgument> outputs_;
  // ExecContext exec_context_;
  Argument grad_;

  // NOTE All the input arguments should in order.
  std::vector<Edge *> inlinks_;

  std::thread service_thread_;

  // model parameters
  std::vector<ArgumentPtr> weights_;

  Graph *graph_;
};

} // namespace chasky
#endif
