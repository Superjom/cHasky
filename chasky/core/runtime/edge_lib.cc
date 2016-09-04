#include <sstream>
#include <regex>

#include "chasky/core/common/status.h"
#include "chasky/core/runtime/edge_lib.h"
namespace chasky {

string EdgeLib::CreateKey(const string &src_node, const string &src_arg,
                          const string &trg_node, const string &trg_arg) {
  return strings::Printf("%s:%s->%s:%s", src_node.c_str(), src_arg.c_str(),
                         trg_node.c_str(), trg_arg.c_str());
}

string EdgeLib::CreateKey(const EdgeDef &edge) {
  return strings::Printf("%s:%s->%s:%s", edge.src_node().c_str(),
                         edge.src_arg().c_str(), edge.trg_node().c_str(),
                         edge.trg_arg().c_str());
}

Status EdgeLib::ParseKey(const string &key, EdgeDef *edge) {
  Status status;

  std::regex re("(\\w+):(\\w+)->(\\w+):(\\w+)");
  std::smatch match;
  CHECK(std::regex_search(key.begin(), key.end(), match, re));
  CHECK_EQ(match.size(), 5);
  edge->set_src_node(match[1]);
  edge->set_src_arg(match[2]);
  edge->set_trg_node(match[3]);
  edge->set_trg_arg(match[4]);

  return status;
}

string EdgeLib::CreateArgKey(const string &node, const string &arg) {
  return strings::Printf("%s:%s", node.c_str(), arg.c_str());
}

Status EdgeLib::Register(const string &key) {
  Status status;
  std::unique_ptr<EdgeDef> edge(new EdgeDef);
  CH_CHECK_OK(ParseKey(key, edge.get()));
  edges_[key] = std::move(edge);
  // CH_STEST_RETURN2(edges_.insert({key, edge}).second, error::UNKNOWN,
  //                  "insert record failed");
  // register by source
  string source_key = CreateArgKey(edge->src_node(), edge->src_arg());
  string target_key = CreateArgKey(edge->trg_node(), edge->trg_arg());

#define REGISTER_EDGE(EDGES, KEY, VAL)                                         \
  CH_STEST_RETURN2(EDGES.count(KEY) == 0, error::INVALID_ARGUMENT,             \
                   "a key called %s has been registered.", KEY.c_str());       \
  EDGES[KEY] = VAL;
  REGISTER_EDGE(src_edges_, source_key, target_key);
  REGISTER_EDGE(trg_edges_, target_key, source_key);
#undef REGISTER_EDGE

  return status;
}

Status EdgeLib::RetriveBySource(const string &key, string *arg_sign) {
  Status status;
  auto it = src_edges_.find(key);
  CH_STEST_RETURN2(it != src_edges_.end(), error::OUT_OF_RANGE,
                   "no source key called %s", key.c_str());
  *arg_sign = it->second;
  return status;
}

Status EdgeLib::RetriveByTarget(const string &key, string *arg_sign) {
  Status status;
  auto it = trg_edges_.find(key);
  CH_STEST_RETURN2(it != src_edges_.end(), error::OUT_OF_RANGE,
                   "no source key called %s", key.c_str());
  *arg_sign = it->second;
  return status;
}

string EdgeLib::DebugString() const {
  std::stringstream ss;
  ss << "EdgeLib:";
  for (const auto &item : edges_) {
    ss << item.first << std::endl;
  }
  return ss.str();
}

} // namespace chasky
