#ifndef CHASKY_CORE_FRAMEWORK_NODE_H_
#define CHASKY_CORE_FRAMEWORK_NODE_H_
#include <memory>
#include <thread>
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/function.h"
#include "chasky/core/framework/argument.h"
namespace chasky {

class Node;

enum TaskType { FORWORD, BACKWARD };

// Generate edge's signature
// @node1: source node's name
// @node2: target node's name
// @arg: source's argument's name
std::string GenEdgeKey(const std::string &node1, const std::string &node2,
                       const std::string &arg);

// Generate edge's signature
// @input: format like "%s:%s" % (node1, arg)
// @node2: target node's name
std::string GenEdgeKey(const std::string &input, const std::string &node2);

class Edge {
public:
  Edge(const Node *src, const Node *trg, const std::string &arg);

  // Consumers wait for activatioin/gradient ready.
  void Consume(TaskType task) const {
    std::unique_lock<std::mutex> lock(mu_);
    forward_ready_cond_.wait(lock, [this, task] {
      if (task == FORWORD)
        return forward_is_ready_;
      return backward_is_read_;
    });
  }

  // Set Edge ready and notify all the consumers that the edge is ready.
  void SetReady(TaskType task) const {
    forward_is_ready_ = true;
    std::condition_variable &cond =
        (task == FORWORD) ? forward_ready_cond_ : backward_ready_cond_;
    cond.notify_all();
  }

  // Clear all states
  void Clear() const {
    forward_is_ready_ = false;
    backward_is_read_ = false;
  }

  // Attribute getters
  const Node *Source() const { return src_; }
  const Node *Target() const { return trg_; }
  StringPiece Signature() const { return signature_; }
  Argument &Activation() { return activation_; }
  Argument &Gradient() { return gradient_; }
  const Argument &Activation() const { return activation_; }
  const Argument &Gradient() const { return gradient_; }

private:
  // format: "%s->%s" % (srcNode.name, destNode.name)
  std::string signature_;
  // forward product
  Argument activation_;
  // backward product
  Argument gradient_;

  const Node *src_;
  const Node *trg_;

  // asynchronous control.
  mutable std::mutex mu_;
  mutable std::condition_variable forward_ready_cond_;
  mutable std::condition_variable backward_ready_cond_;
  // Mark whether activation is ready, should be reset false after each turn.
  mutable bool forward_is_ready_ = false;
  // Mark whether gradient is ready, should be reset false after each turn.
  mutable bool backward_is_read_ = false;
};

// Node is a node of compute graph. It is the carrier of the corresponding
// function.
// Node starts a thread as the function's service. The Service will receive
// Edges' tasks and start a ForwardCompute or BackwardCompute task.
// After Node's task is finished, the output arguments will be released to the
// out-link edges, and other nodes will receive this arguments.
class Node {
public:
  Node(){};
  // Create a node, including create the corresponding function's object.
  Node(const NodeDef &def);

  ~Node();

  // Create a node from definition
  static std::unique_ptr<Node> Create(const NodeDef &def);

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

protected:
  // Tell the edge's endpoints that the argument is ready.
  Status ReleaseEdge(const std::string &edge);
  Status ReleaseEdge(const Edge *edge);

private:
  // Current task
  TaskType cur_task_;
  // the corresponding function
  std::unique_ptr<Function> func_;
  NodeDef def_;
  FunctionDef func_def_;
  // ExecContext exec_context_;
  Argument grad_;
  std::thread service_thread_;
};
}
#endif
