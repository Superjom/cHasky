#include "chasky/core/framework/graph.h"
namespace chasky {

std::unique_ptr<Graph> Graph::Create() {
  return std::unique_ptr<Graph>(new Graph);
}

std::shared_ptr<Argument> Graph::Param(const std::string &name) {
  auto it = params_.find(name);
  if (it == params_.end())
    return nullptr;
  return it->second;
}

} // namespace chasky
