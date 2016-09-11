#include <sstream>
#include "chasky/core/runtime/node_lib.h"
namespace chasky {

Status NodeLib::RegisterCreate(const string &key, const NodeDef &def) {
  Status status;
  auto node = Node::Create(def, postbox_, edge_lib_);
  CH_STEST_RETURN2(nodes_.count(key) == 0, error::INVALID_ARGUMENT,
                   "an key called %s already exists, register node failed",
                   key.c_str());
  nodes_[key] = std::move(node);
  return status;
}

Status NodeLib::Retrive(const string &key, Node **node) const {
  Status status;
  auto it = nodes_.find(key);
  CH_STEST_RETURN2(it != nodes_.end(), error::INVALID_ARGUMENT,
                   "no node called %s exists.", key.c_str());
  *node = it->second.get();
  return status;
}

string NodeLib::DebugString() const {
  std::stringstream ss;
  ss << "NodeLib:" << std::endl;
  for (const auto &item : nodes_) {
    ss << item.first << std::endl;
  }
  return ss.str();
}

} // namespace chasky
