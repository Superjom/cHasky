#ifndef CHASKY_CORE_FRAMEWORK_EDGE_H
#define CHASKY_CORE_FRAMEWORK_EDGE_H
#include "chasky/core/framework/argument.h"
#include "chasky/core/framework/graph.pb.h"
namespace chasky {

class Node;
class Edge;
using edge_ptr_t = std::shared_ptr<Edge>;

enum class TaskType { FORWORD, BACKWARD };

// An Edge is a connection between two argument of different nodes, it can be
// identified by "{node1:arg1}->{node2:arg2}", which means node2 has an input
// called 'arg2', and its real value is copy/clone from node1's
// output(activation) arg1.
// An node should not start computation until all its edges' data are ready.
// Edge will carry the source node's output(activation) to transfer data to
// target node, it will also carry source node's gradient argument to make it
// possible for target node to send back gradient.
class Edge {
public:
  explicit Edge() {}
  // An edge is like "source_node.output_arg -> target_node.input_arg", it
  // identifies an argument to another.
  // NOTE Edge connects different nodes by Argument, an node will have multiple
  // edges if it has more than one argument.
  explicit Edge(const Node *src, const std::string &src_arg, const Node *trg,
                const std::string &trg_arg);

  explicit Edge(const EdgeDef &def, const Node *src, const Node *trg)
      : signature_(def.signature_()),
        src_sign_(strings::Printf("%s:%s", def.src_node().c_str(),
                                  def.src_arg().c_str())),
        trg_sign_(strings::Printf("%s:%s", def.trg_node().c_str(),
                                  def.trg_arg().c_str())),
        src_(src), trg_(trg) {}

  // Consumers wait for activatioin/gradient ready.
  // TODO exit when task is killed
  void Consume(TaskType task) const {
    std::unique_lock<std::mutex> lock(mu_);
    forward_ready_cond_.wait(lock, [this, task] {
      if (task == TaskType::FORWORD)
        return forward_is_ready_;
      return backward_is_read_;
    });
  }

  // Set Edge ready and notify all the consumers that the edge is ready.
  void SetReady(TaskType task) const {
    forward_is_ready_ = true;
    std::condition_variable &cond = (task == TaskType::FORWORD)
                                        ? forward_ready_cond_
                                        : backward_ready_cond_;
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
  Argument &Activation() { return *activation_; }
  Argument &Gradient() { return *gradient_; }
  const Argument &Activation() const { return *activation_; }
  const Argument &Gradient() const { return *gradient_; }

private:
  // format: "%s->%s" % (srcNode.name, destNode.name)
  std::string signature_;
  // format: "{src_node}:{arg_name}"
  std::string src_sign_;
  // format: "{trg_node}:{arg_name}"
  std::string trg_sign_;
  // forward product
  ArgumentPtr activation_;
  // backward product
  ArgumentPtr gradient_;

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
