/*
 * Edges' library.
 *
 * Sat Sep  3 10:24:45 CST 2016
 */
#ifndef CHASKY_CORE_RUNTIME_EDGE_LIB_H_
#define CHASKY_CORE_RUNTIME_EDGE_LIB_H_
#include <memory>
#include <unordered_map>

#include "chasky/core/common/status.h"
#include "chasky/core/framework/graph.pb.h"

namespace chasky {
using std::string;

class EdgeLib {
public:
  static string CreateKey(const string &src_node, const string &src_arg,
                          const string &trg_node, const string &trg_arg);
  static string CreateKey(const EdgeDef &edge);

  static Status ParseKey(const string &key, EdgeDef *parsed_key);

  static string CreateArgKey(const string &node, const string &arg);

  // Register an edge by key create by CreateKey
  Status Register(const string &key);
  // Retrive args' key by source point.
  Status RetriveBySource(const string &key, string *arg_sign);
  // Retrive args' key by target point.
  Status RetriveByTarget(const string &key, string *arg_sign);

  string DebugString() const;

  bool IsEmpty() const { return edges_.empty(); }

private:
  // key to edges
  std::unordered_map<string, std::unique_ptr<EdgeDef>> edges_;
  // src's edges
  std::unordered_map<string, string> src_edges_;
  // trg's edges
  std::unordered_map<string, string> trg_edges_;
};

// struct Edge {
//   string src_node;
//   string src_arg;
//   string trg_node;
//   string trg_arg;
// };

typedef EdgeDef *edge_raw_ptr_t;
typedef std::shared_ptr<EdgeDef> edge_ptr_t;

} // namespace chasky
#endif
