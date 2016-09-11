#ifndef CHASKY_CORE_FRAMEWORK_NODE_DEF_BUILDER_H_
#define CHASKY_CORE_FRAMEWORK_NODE_DEF_BUILDER_H_
#include "chasky/core/framework/attr_value.pb.h"
#include "chasky/core/framework/graph.pb.h"
namespace chasky {

class NodeDefBuilder {
public:
  explicit NodeDefBuilder() {}

  NodeDefBuilder &Name(const std::string &x) {
    def_.set_name(x);
    return *this;
  }

  NodeDefBuilder &SetIsSource(bool x) {
    def_.set_is_source(x);
    return *this;
  }

  NodeDefBuilder &SetIsEnd(bool x) {
    def_.set_is_end(x);
    return *this;
  }

  NodeDefBuilder &Signature(const std::string &x) {
    def_.set_signature(x);
    return *this;
  }

  NodeDefBuilder &Input(const std::string &x) {
    *def_.add_input() = x;
    return *this;
  }

  NodeDefBuilder &Attr(const std::string &name, const AttrValue &x) {
    (*def_.mutable_attr())[name] = x;
    return *this;
  }

  NodeDef Finalize() const {
    // TODO add some valid here.
    return def_;
  }

private:
  NodeDef def_;
}; // NodeDefBuilder

} // namespace chasky
#endif
