#ifndef CHASKY_CORE_RUNTIME_STATE_H_
#define CHASKY_CORE_RUNTIME_STATE_H_
#include "chasky/core/common/status.h"
// All the states need to manage in the runtime.
namespace chasky {

class RuntimeState {
private:
  // Tell all the components in a computation graph to keep running or not(task
  // is killed).
  bool keep_running_ = true;

  // Keep updated during exectuation, will be checked every time after a
  // complement finished its task. If the returned status is failure, the whole
  // task should be killed.
  Status status_;

  // Protect the states.
  // TODO replace this with spin_lock?
  mutable std::mutex mu_;

public:
  // NOTE This singleton should not be used?
  // A Session should has its own RuntimeState, not a global one.
  static RuntimeState &Instance() {
    static RuntimeState *x = new RuntimeState;
    return *x;
  }

  void Kill() { UpdateStatus(Status{error::KILLED, "Killed"}); }

  bool KeepRunning() const { return keep_running_; }

  // Update the status with an returned one. If the status is failure, kill the
  // task.
  void UpdateStatus(const Status &other) {
    {
      std::lock_guard<std::mutex> lk(mu_);
      status_.Update(other);
    }

    if (!status_.ok()) {
      // tell other components to break task.
      keep_running_ = false;
    }
  }
};
}
#endif
