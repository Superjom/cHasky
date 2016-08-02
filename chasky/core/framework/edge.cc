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
      src_(src), trg_(trg) {

  // set src_sign and trg_sign
  char src_sign[100], trg_sign[100];
  int success_args =
      std::scanf(signature_.c_str(), "%s->%s", src_sign, trg_sign);
  CHECK_EQ(success_args, 2) << "signature_ do not match the format "
                               "'{src_node}:{arg_name}->{trg_node}:{arg_name}'";
  src_sign_ = src_sign;
  trg_sign_ = trg_sign;
}

} // namespace chasky
