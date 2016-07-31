#include "chasky/core/common/status.h"
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

Status Graph::RegisterEdge(StringPiece sign, edge_ptr_t edge) {
  Status status;
  DLOG(INFO) << "register edge called " << sign;
  std::string signature = sign.tostring();
  auto it = edges_.find(signature);
  CH_STEST_RETURN2(it != edges_.end(), error::INVALID_ARGUMENT,
                   "an edge called %s already exists", signature.c_str());
  CH_STEST_RETURN2(edges_.insert({signature, edge}).second, error::UNKNOWN,
                   "edges_ insert map error");
  // register by source and target
  char src_sign[100], trg_sign[100];
  int success_args =
      std::scanf(signature.c_str(), "%s->%s", src_sign, trg_sign);
  CH_STEST_RETURN2(success_args == 2, error::INVALID_ARGUMENT,
                   "signature [%s] do not match the format "
                   "'{src_node}:{arg1}->{trg_node}:{arg2}'",
                   signature.c_str());

#define REGISTER_EDGE(EDGES, KEY)                                              \
  if (EDGES.find(KEY) == EDGES.end()) {                                        \
    EDGES.emplace(KEY, std::vector<edge_ptr_t>());                             \
  }                                                                            \
  EDGES[KEY].push_back(edge);

  REGISTER_EDGE(trg2edges_, trg_sign)
  REGISTER_EDGE(src2edges_, src_sign)
#undef REGISTER_EDGE

  return status;
}

Status Graph::GetEdgeBySign(StringPiece sign, edge_ptr_t *edge) {
  Status status;
  std::string signature = sign.tostring();
  auto it = edges_.find(signature);
  CH_STEST_RETURN2(it != edges_.end(), error::OUT_OF_RANGE,
                   "no edges exists called [%s]", signature.c_str());
  *edge = it->second;
  return status;
}

Status Graph::GetEdgesByTarget(const std::string &signature,
                               std::vector<edge_ptr_t> **edges) {
  Status status;
  auto it = trg2edges_.find(signature);
  CH_STEST_RETURN2(it != trg2edges_.end(), error::OUT_OF_RANGE,
                   "no key called %s exists in trg2edges_", signature.c_str());
  *edges = &it->second;
  return status;
}

Status Graph::GetEdgeByTarget(const std::string &signature, edge_ptr_t *edge) {
  std::vector<edge_ptr_t> *edges;
  Status status = GetEdgesByTarget(signature, &edges);
  if (!status.ok())
    return status;
  CH_STEST_RETURN2(
      edges->size() == 1, error::OUT_OF_RANGE,
      "there are [%lu] edges registered by target [%s] is registered",
      edges->size(), signature.c_str());
  *edge = edges->at(0);
  return status;
}

} // namespace chasky
