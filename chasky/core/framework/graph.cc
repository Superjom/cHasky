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
  CH_STEST_RETURN2(it == edges_.end(), error::INVALID_ARGUMENT,
                   "an edge called %s already exists", signature.c_str());
  CH_STEST_RETURN2(edges_.insert({signature, edge}).second, error::UNKNOWN,
                   "edges_ insert map error");

#define REGISTER_EDGE(EDGES, KEY)                                              \
  if (EDGES.find(KEY) == EDGES.end()) {                                        \
    EDGES.emplace(KEY, std::vector<edge_ptr_t>());                             \
  }                                                                            \
  EDGES[KEY].push_back(edge);

  REGISTER_EDGE(src2edges_, edge->SrcSign().tostring())
  REGISTER_EDGE(trg2edges_, edge->TrgSign().tostring())
#undef REGISTER_EDGE

  return status;
}

Status Graph::GetEdgeBySign(StringPiece sign, Edge **edge) {
  Status status;
  std::string signature = sign.tostring();
  auto it = edges_.find(signature);
  CH_STEST_RETURN2(it != edges_.end(), error::OUT_OF_RANGE,
                   "no edges exists called [%s]", signature.c_str());
  *edge = it->second.get();
  return status;
}

Status Graph::GetEdgesByTarget(const std::string &signature,
                               std::vector<edge_ptr_t> **edges) {
  Status status;
  auto it = trg2edges_.find(signature);
  CH_STEST_RETURN2(it != trg2edges_.end(), error::OUT_OF_RANGE,
                   "no key called %s in trg2edges_", signature.c_str());
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

Status Graph::GetEdgesBySource(const std::string &signature,
                               std::vector<edge_ptr_t> **edges) {
  Status status;
  auto it = src2edges_.find(signature);
  CH_STEST_RETURN2(it != src2edges_.end(), error::OUT_OF_RANGE,
                   "no key called %s exists in src2edges_", signature.c_str());
  *edges = &it->second;
  return status;
}

} // namespace chasky
