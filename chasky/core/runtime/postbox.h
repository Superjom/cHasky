#ifndef CHASKY_CORE_RUNTIME_POSTBOX_H_
#define CHASKY_CORE_RUNTIME_POSTBOX_H_

#include <string>
#include <memory>
#include <unordered_map>

#include "chasky/core/runtime/edge_lib.h"
#include "chasky/core/framework/argument.h"

namespace chasky {
using std::string;

/*
 * A PostBox is an abstraction for argument transfer between producer and
 * consumer
 */
class PostBox {
public:
  // Create an key(signature) for this send behavior.
  // static string CreateKey(const string &source_node, const string
  // &source_arg,
  //                         const string &target_node, const string &arg_name);

  // Create an key(signature) for an argument item.
  // Format like: {node_name}:{arg_name}
  static string CreateArgKey(const string &node_name, const string &arg_name);

  // Register an empty argument item.
  Status Register(const string &key, Argument *ptr = nullptr);

  // Key should be created by CreateArgKey
  Status Send(const string &key, Argument *arg);

  static Status ParseKey(const string &key, string *node, string *arg);

  // Consumer's callback to deal with the received argument.
  typedef std::function<void(Argument *arg)> ReadyCallback;

  // Consumer consume an argument, and register its callback. Consumer's thread
  // will be blocked until the argument is ready.
  // key's format is {node_name}:{arg_name}
  Status Consume(const string &key, ReadyCallback &&callback);

  // Abort all parameter's transfer.
  Status Abort();

  std::string DebugString() const;
  class ArgItem {
  public:
    void SetReady(Argument *arg) {
      DLOG(INFO) << "set ready";
      arg_ = arg;
      is_ready_ = true;
      for (auto &callback : ready_callbacks_) {
        callback(arg);
      }
    }

    void SetUnready() {
      arg_ = nullptr;
      is_ready_ = false;
    }

    void Consume(ReadyCallback &&callback) {
      ready_callbacks_.push_back(callback);
    }

    bool IsReady() const { return is_ready_; }

    Argument *Arg() { return arg_; }

    Argument *&mutable_arg() { return arg_; }

  private:
    Argument *arg_;
    bool is_ready_ = false;
    std::vector<ReadyCallback> ready_callbacks_;
  };

private:
  // key: argument's key like: {source_node_name}:{argument_name}
  std::unordered_map<string, ArgItem> args_;
};
}
#endif
