#include "chasky/common/strings.h"
#include "chasky/core/framework/edge_utils.h"
namespace chasky {

std::string GenEdgeKey(const std::string &source, const std::string &src_arg,
                       const std::string &target, const std::string &trg_arg) {
  return strings::Printf("%s:%s->%s:%s", source.c_str(), src_arg.c_str(),
                         target.c_str(), trg_arg.c_str());
}

}
