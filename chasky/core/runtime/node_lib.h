/*
 * Nodes' library.
 *
 *Sat Sep  3 10:13:04 CST 2016
 */
#include <memory>
#include <unordered_map>

#include "chasky/core/framework/node.h"
#include "chasky/core/runtime/postbox.h"
#include "chasky/core/runtime/edge_lib.h"
#include "chasky/core/framework/graph.pb.h"

namespace chasky {

class NodeLib {
public:
  NodeLib(PostBox *postbox, EdgeLib *edge_lib)
      : postbox_(postbox), edge_lib_(edge_lib) {}
  // Register and create a node by name and definition.
  Status RegisterCreate(const string &key, const NodeDef &def);
  // Retrieve a node by name.
  Status Retrive(const string &key, Node **node) const;

  bool IsEmpty() const { return nodes_.empty(); }

  std::unordered_map<string, std::unique_ptr<Node>> &Nodes() { return nodes_; }
  // Human-readable debug information.
  string DebugString() const;

private:
  PostBox *postbox_;
  EdgeLib *edge_lib_;
  std::unordered_map<string, std::unique_ptr<Node>> nodes_;
};

} // namespace chasky
