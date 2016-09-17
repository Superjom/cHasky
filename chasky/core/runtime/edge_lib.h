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
// EdgeLib is a dictionary of edges. Edges are stored like "key :
// representation". Because computation have two mode: forward and backward.
class EdgeLib {
public:
  // create edge's key in edge_lib
  // src_arg: source argument's key
  // trg_arg: target argument's key
  static string CreateKey(const string &src_arg, const string &trg_arg);

  static string CreateKey(const EdgeDef &edge);

  static Status ParseKey(const string &key, EdgeDef *parsed_key);
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

typedef EdgeDef *edge_raw_ptr_t;
typedef std::shared_ptr<EdgeDef> edge_ptr_t;

} // namespace chasky
#endif
