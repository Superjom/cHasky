#include <gtest/gtest.h>
#include "chasky/core/framework/graph.h"
#include "chasky/core/framework/graph_utils.h"
namespace chasky {
namespace test {

class GraphTest : public ::testing::Test {
public:
  void SetUp() {
    CreateGraphDef();

    graph_ = Graph::Create();
  }

  void CreateGraphDef() {
    NodeDef node1 = NodeDefBuilder()
                        .Name("data")
                        .IsSource(true)
                        .IsEnd(false)
                        .Signature("AddN:float_mat")
                        .Finalize();

    NodeDef node2 = NodeDefBuilder()
                        .Name("node1")
                        .IsSource(false)
                        .IsEnd(false)
                        .Signature("AddN:float_mat")
                        .Finalize();

    EdgeDef edge1 = EdgeDefBuilder()
                        .SrcNode("data")
                        .SrcArg("output1")
                        .TrgNode("node1")
                        .TrgArg("input1")
                        .Finalize();

    graph_def_ = GraphDefBuilder()
                     .Name("graph1")
                     .AddNodeDef(node1)
                     .AddNodeDef(node2)
                     .AddEdgeDef(edge1)
                     .Finalize();
  }

protected:
  graph_ptr_t graph_;
  GraphDef graph_def_;
};

TEST_F(GraphTest, setup) {}

TEST_F(GraphTest, RegisterEdge) {

  EdgeDef edge_def = EdgeDefBuilder()
                         .SrcNode("data")
                         .SrcArg("output1")
                         .TrgNode("node1")
                         .TrgArg("input1")
                         .Finalize();
  EdgeDef edge_def1 = EdgeDefBuilder()
                          .SrcNode("node1")
                          .SrcArg("output3")
                          .TrgNode("node1")
                          .TrgArg("input1")
                          .Finalize();

  auto edge1 = std::make_shared<Edge>(edge_def, nullptr, nullptr);
  EXPECT_TRUE(graph_->RegisterEdge(edge_def.signature_(), edge1).ok());

  auto edge2 = std::make_shared<Edge>(edge_def1, nullptr, nullptr);
  EXPECT_TRUE(graph_->RegisterEdge(edge_def1.signature_(), edge2).ok());

  std::vector<edge_ptr_t> *edges1 = nullptr;
  EXPECT_TRUE(graph_->GetEdgesByTarget("node1:input1", &edges1).ok());
  EXPECT_EQ(edges1->size(), 2LU);

  EXPECT_TRUE(graph_->GetEdgesBySource("data:output1", &edges1).ok());
  EXPECT_EQ(edges1->size(), 1LU);

  EXPECT_TRUE(graph_->GetEdgesBySource("node1:output3", &edges1).ok());
  EXPECT_EQ(edges1->size(), 1LU);

  Edge *edge = nullptr;
  EXPECT_TRUE(graph_->GetEdgeBySign(edge_def1.signature_(), &edge).ok());
}

} // namespace test
} // namespace chasky
