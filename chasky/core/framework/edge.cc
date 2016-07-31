#include "chasky/core/framework/edge.h"
#include "chasky/core/framework/node.h"
namespace chasky {

std::string GenEdgeKey(const std::string &source, const std::string &src_arg,
                       const std::string &target, const std::string &trg_arg) {
  return strings::Printf("%s:%s->%s:%s", source.c_str(), src_arg.c_str(),
                         target.c_str(), trg_arg.c_str());
}

// std::string GenEdgeKey(const std::string &input, const std::string &node2) {
//   return strings::Printf("%s->%s", input.c_str(), node2.c_str());
// }

Edge::Edge(const Node *src, const std::string &src_arg, const Node *trg,
           const std::string &trg_arg)
    : signature_(GenEdgeKey(src->Name().tostring(), src_arg.c_str(),
                            trg->Name().tostring(), trg_arg)),
      src_(src), trg_(trg) {}
} // namespace chasky
