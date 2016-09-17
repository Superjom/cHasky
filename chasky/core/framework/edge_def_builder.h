#ifndef CHASKY_CORE_FRAMEWORK_EDGE_DEF_BUILDER_H_
#define CHASKY_CORE_FRAMEWORK_EDGE_DEF_BUILDER_H_

#include "chasky/core/framework/graph.pb.h"

namespace chasky {
using namespace std;
class EdgeDefBuilder {
public:
  explicit EdgeDefBuilder() {}

  EdgeDefBuilder &SrcNode(const string &x) {
    *def_.mutable_src_node() = x;
    return *this;
  }

  EdgeDefBuilder &SrcArg(const string &x) {
    *def_.mutable_src_arg() = x;
    return *this;
  }

  EdgeDefBuilder &TrgNode(const string &x) {
    *def_.mutable_trg_node() = x;
    return *this;
  }

  EdgeDefBuilder &TrgArg(const string &x) {
    *def_.mutable_trg_arg() = x;
    return *this;
  }

  EdgeDefBuilder &Mode(TaskType mode) {
    def_.set_mode(mode);
    return *this;
  }

  EdgeDef &Finalize() { return def_; }

private:
  EdgeDef def_;
};
} // namespace chasky
#endif
