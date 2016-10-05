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
  // Format like: {node_name}:{arg_name}_{mode}
  // mode:
  //     - f(forward)
  //     - b(bckword)
  static string CreateArgKey(const string &node_name, const string &arg_name,
                             TaskType mode = FORWARD);

  static Status ParseKey(const string &key, string *node, string *arg,
                         TaskType *mode);

  // Register an empty argument item.
  Status Register(const string &key, ArgumentPtr ptr = nullptr,
                  bool set_ready = false);

  // Key should be created by CreateArgKey
  Status Send(const string &key, ArgumentPtr arg);

  // Consumer's callback to deal with the received argument.
  typedef std::function<void(ArgumentPtr arg)> ReadyCallback;

  // Consumer consume an argument, and register its callback. Consumer's thread
  // will be blocked until the argument is ready.
  // key's format is {node_name}:{arg_name}
  Status Consume(const string &key, ReadyCallback &&callback);

  // Consumer consume an argument without wait, return error::NOT_INITED if
  // argument is not ready.
  Status Consume(const string &key, ArgumentPtr *arg);

  // force consume the argument, no regard of the ready status.
  Status ForceConsume(const string&key, ArgumentPtr*arg);

  // Abort all parameter's transfer.
  Status Abort();

  std::string DebugString() const;

  class ArgItem {
  public:
    ArgItem() {}

    void SetLock(std::mutex *lock) {
      CHECK(lock);
      lock_ = lock;
    }

    void SetReady(ArgumentPtr arg) {
      arg_ = arg;
      is_ready_ = true;
      std::lock_guard<std::mutex> lock(*lock_);
      for (auto &callback : ready_callbacks_) {
        callback(arg);
      }
      ready_callbacks_.clear();
      // avoid loop
      // SetUnready();
    }

    void SetReady() {
      is_ready_ = true;
    }

    void SetUnready() {
      // arg_ = nullptr;
      is_ready_ = false;
    }

    void Consume(ReadyCallback &&callback) {
      std::lock_guard<std::mutex> lock(*lock_);
      ready_callbacks_.push_back(callback);
    }

    bool IsReady() const { return is_ready_; }

    ArgumentPtr Arg() { return arg_; }

    void SetArgument(ArgumentPtr x) { arg_ = x; }

  private:
    ArgumentPtr arg_;
    std::mutex *lock_;
    bool is_ready_ = false;
    std::vector<ReadyCallback> ready_callbacks_;
  };

private:
  // key: argument's key like: {source_node_name}:{argument_name}
  std::unordered_map<string, ArgItem> args_;
  std::mutex arg_item_lock_;
  std::mutex args_lock_;
};
}
#endif
