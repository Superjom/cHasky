#ifndef CHASKY_CORE_COMMON_STATUS_H_
#define CHASKY_CORE_COMMON_STATUS_H_
#include <string>
#include <glog/logging.h>
#include "chasky/common/strings.h"
namespace chasky {

namespace error {
enum Code {
  // success
  OK = 0,
  UNKNOWN = 1,
  INVALID_ARGUMENT = 2,
  OUT_OF_RANGE = 3,
  UNIMPLEMENTED = 4
};
} // namespace error

class Status {
public:
  Status() : state_(nullptr) {}

  explicit Status(error::Code code, const std::string &msg)
      : state_(new State({code, msg})) {}

  bool ok() const { return state_ == nullptr; }

  error::Code code() const {
    if (ok())
      return OK();
    return state_->code;
  }

  void Update(const Status &other) {
    if (other.ok())
      return;
    if (state_ == nullptr) {
      state_.reset(new State({other.code(), other.msg()}));
    }
  }

  static error::Code OK() { return error::Code::OK; }

  const std::string &msg() const {
    if (ok()) {
      return strings::empty_string;
    } else {
      return state_->msg;
    }
  }

  bool operator==(const Status &x) const;
  bool operator!=(const Status &x) const;

private:
  struct State {
    error::Code code;
    std::string msg;
  };

  std::unique_ptr<State> state_;
}; // class Status

#define CH_CHECK_OK(val) CHECK((val).ok()) << "error: " << (val).msg();

} // namespace chasky
#endif
