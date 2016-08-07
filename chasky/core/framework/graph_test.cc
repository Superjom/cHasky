#include <gtest/gtest.h>
#include "chasky/core/framework/graph.h"
#include "chasky/core/framework/graph_utils.h"
namespace chasky {
namespace test {

class GraphTest : public ::testing::Test {
public:
  void SetUp() {
    graph_ = Graph::Create();

    CreateGraphDef();
  }

  void CreateGraphDef() {
    edge_def = EdgeDefBuilder()
                           .SrcNode("data")
                           .SrcArg("output1")
                           .TrgNode("node1")
                           .TrgArg("input1")
                           .Finalize();
    edge_def1 = EdgeDefBuilder()
                            .SrcNode("node1")
                            .SrcArg("output3")
                            .TrgNode("node1")
                            .TrgArg("input1")
                            .Finalize();

    auto edge1 = std::make_shared<Edge>(edge_def, nullptr, nullptr);
    EXPECT_TRUE(graph_->RegisterEdge(edge_def.signature_(), edge1).ok());

    auto edge2 = std::make_shared<Edge>(edge_def1, nullptr, nullptr);
    EXPECT_TRUE(graph_->RegisterEdge(edge_def1.signature_(), edge2).ok());
  }

protected:
  EdgeDef edge_def;
  EdgeDef edge_def1;
  graph_ptr_t graph_;
};

TEST_F(GraphTest, setup) {}

TEST_F(GraphTest, RegisterEdge) {

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
