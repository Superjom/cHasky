#include <sstream>
#include <regex>

#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/common/status.h"
#include "chasky/core/runtime/edge_lib.h"
#include "chasky/core/runtime/postbox.h"
namespace chasky {

string EdgeLib::CreateKey(const string &src_arg, const string &trg_arg) {
  return strings::Printf("%s->%s", src_arg.c_str(), trg_arg.c_str());
}

string EdgeLib::CreateKey(const EdgeDef &edge) {
  return strings::Printf("%s:%s:%d->%s:%s:%d", edge.src_node().c_str(),
                         edge.src_arg().c_str(), edge.mode(),
                         edge.trg_node().c_str(), edge.trg_arg().c_str(),
                         edge.mode());
}

Status EdgeLib::ParseKey(const string &key, EdgeDef *edge) {
  Status status;

  std::regex re("([0-9a-z_-]+):([0-9a-z_-]+):([0-9]+)->([0-9a-z_-]+):([0-9a-z_-"
                "]+):([0-9]+)");
  std::smatch match;
  CHECK(std::regex_search(key.begin(), key.end(), match, re)) << key;
  CHECK_EQ(match.size(), 7);
  edge->set_src_node(match[1]);
  edge->set_src_arg(match[2]);
  edge->set_trg_node(match[4]);
  edge->set_trg_arg(match[5]);
  edge->set_signature_(key);
  edge->set_mode((TaskType)std::stoi(match[6].str()));
  return status;
}

Status EdgeLib::Register(const string &key) {
  DLOG(INFO) << "register edge to EdgeLib: " << key;
  Status status;
  std::unique_ptr<EdgeDef> _edge(new EdgeDef);
  CH_CHECK_OK(ParseKey(key, _edge.get()));
  // DLOG(INFO) << strings::Printf("register edge [%s] to edges_", key.c_str());
  edges_[key] = std::move(_edge);
  // DLOG(INFO) << strings::Printf("finish registering edge [%s] to edges_",
  //                               key.c_str());
  // CH_STEST_RETURN2(edges_.insert({key, edge}).second, error::UNKNOWN,
  //                  "insert record failed");
  // register by source
  auto &edge = edges_[key];
  string source_key =
      PostBox::CreateArgKey(edge->src_node(), edge->src_arg(), edge->mode());
  string target_key =
      PostBox::CreateArgKey(edge->trg_node(), edge->trg_arg(), edge->mode());

  DLOG(INFO) << strings::Printf("register edge [%s] to src_edges_",
                                source_key.c_str());
  DLOG(INFO) << strings::Printf("register edge [%s] to trg_edges_",
                                target_key.c_str());

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
