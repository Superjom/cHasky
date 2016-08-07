#ifndef CHASKY_CORE_FRAMEWORK_EDGE_H_
#define CHASKY_CORE_FRAMEWORK_EDGE_H_
#include <string>
namespace chasky {
// Generate edge's signature
// @node1: source node's name
// @node2: target node's name
// @arg: source's argument's name
std::string GenEdgeKey(const std::string &source, const std::string &src_arg,
                       const std::string &target, const std::string &trg_arg);

// Generate edge's signature
// @input: format like "%s:%s" % (node1, arg)
// @node2: target node's name
std::string GenEdgeKey(const std::string &input, const std::string &node2);
}
#endif
