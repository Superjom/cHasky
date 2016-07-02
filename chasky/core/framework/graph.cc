#include "chasky/core/framework/graph.h"
namespace chasky {

std::unique_ptr<Graph> Create() {
  return std::unique_ptr<Graph>(new Graph);
}

}
 
