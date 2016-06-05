#include "chasky/core/common/status.h"
using namespace chasky;

bool Status::operator==(const Status& x) const {
  return code() == x.code() && msg() == x.msg();
}

bool Status::operator!=(const Status& x) const {
  return !(*this == x);
}
