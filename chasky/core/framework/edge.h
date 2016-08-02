#ifndef CHASKY_CORE_FRAMEWORK_EDGE_H
#define CHASKY_CORE_FRAMEWORK_EDGE_H
#include "chasky/core/framework/argument.h"
namespace chasky {

class Node;
class Edge;
using edge_ptr_t = std::shared_ptr<Edge>;

enum TaskType { FORWORD, BACKWARD };

// Generate edge's signature
// @node1: source node's name
// @node2: target node's name
// @arg: source's argument's name
std::string GenEdgeKey(const std::string &source, const std::string &src_arg,
                       const std::string &target, const std::string &trg_arg);

// Generate edge's signature
// @input: format like "%s:%s" % (node1, arg)
// @node2: target node's name
std::string GenEdgeKey(const std::string &input, const std::string &node2);

class Edge {
public:
  // An edge is like "source_node.output_arg -> target_node.input_arg", it
  // identifies an argument to another.
  // NOTE Edge connects different nodes by Argument, an node will have multiple
  // edges if it has more than one argument.
  Edge(const Node *src, const std::string &src_arg, const Node *trg,
       const std::string &trg_arg);

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
  StringPiece SrcSign() const { return src_sign_; }
  StringPiece TrgSign() const { return trg_sign_; }
  Argument &Activation() { return activation_; }
  Argument &Gradient() { return gradient_; }
  const Argument &Activation() const { return activation_; }
  const Argument &Gradient() const { return gradient_; }

private:
  // format: "%s->%s" % (srcNode.name, destNode.name)
  std::string signature_;
  // format: "{src_node}:{arg_name}"
  std::string src_sign_;
  // format: "{trg_node}:{arg_name}"
  std::string trg_sign_;
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

} // namespace chasky
#endif
