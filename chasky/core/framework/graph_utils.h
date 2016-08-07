#ifndef CHASKY_CORE_FRAMEWORK_GRAPH_UTILS_H_
#define CHASKY_CORE_FRAMEWORK_GRAPH_UTILS_H_
#include <glog/logging.h>
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/edge_utils.h"
namespace chasky {

class NodeDefBuilder {
public:
  explicit NodeDefBuilder() {}

  NodeDefBuilder &Name(const std::string &x) {
    def_.set_name(x);
    return *this;
  }

  NodeDefBuilder &IsSource(bool x) {
    def_.set_is_source(x);
    return *this;
  }

  NodeDefBuilder &IsEnd(bool x) {
    def_.set_is_end(x);
    return *this;
  }

  NodeDefBuilder &Signature(const std::string &x) {
    def_.set_signature(x);
    return *this;
  }

  // add an attribute
  NodeDefBuilder &Attr(const std::string &key, const AttrValue &val) {
    auto &attrs = *def_.mutable_attr();
    attrs[key] = val;
    return *this;
  }

  const NodeDef &Finalize() {
    // parse def_name from signature
    char def_name[100], dtype[100];
    int nargs =
        std::sscanf(def_.signature().c_str(), "%[^:]:%s", def_name, dtype);
    CHECK_EQ(nargs, 2);
    def_.set_def_name(def_name);
    return def_;
  }

private:
  NodeDef def_;
};

class EdgeDefBuilder {
public:
  explicit EdgeDefBuilder() {}
  EdgeDefBuilder &SrcNode(const std::string &x) {
    def_.set_src_node(x);
    return *this;
  }

  EdgeDefBuilder &SrcArg(const std::string &x) {
    def_.set_src_arg(x);
    return *this;
  }

  EdgeDefBuilder &TrgNode(const std::string &x) {
    def_.set_trg_node(x);
    return *this;
  }

  EdgeDefBuilder &TrgArg(const std::string &x) {
    def_.set_trg_arg(x);
    return *this;
  }

  const EdgeDef &Finalize() {
    auto signature = GenEdgeKey(def_.src_node(), def_.src_arg(),
                                def_.trg_node(), def_.trg_arg());
    def_.set_signature_(signature);
    return def_;
  }

private:
  EdgeDef def_;
};

class GraphDefBuilder {
public:
  explicit GraphDefBuilder() {}

  GraphDefBuilder &Name(const std::string &x) {
    def_.set_name(x);
    return *this;
  }

  GraphDefBuilder &AddNodeDef(const NodeDef &x) {
    *def_.add_nodes() = x;
    return *this;
  }

  GraphDefBuilder &AddEdgeDef(const EdgeDef &x) {
    *def_.add_edges() = x;
    return *this;
  }

  const GraphDef &Finalize() { return def_; }

private:
  GraphDef def_;
};
}
#endif
