#include <gtest/gtest.h>

#include "chasky/core/runtime/session.h"
#include "chasky/core/common/eigen_matrix.h"
#include "chasky/core/framework/attr_value_util.h"
#include "chasky/core/framework/graph_def_builder.h"
#include "chasky/core/framework/node_def_builder.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/edge_def_builder.h"
#include "chasky/core/framework/data_provider_def_builder.h"

namespace chasky {
namespace test {

class SessionTest : public ::testing::Test {
public:
  void CreateGraphDef() {
    auto graph_builder = GraphDefBuilder().Name("demo_graph");

    auto data_provider_def = DataProviderDefBuilder()
                                 .Name("data-provider")
                                 .Field(ArgumentDefBuilder()
                                            .Name("input")
                                            .Type("float_mat")
                                            .Shape(dim, batch_size)
                                            .Finalize())
                                 .Field(ArgumentDefBuilder()
                                            .Name("label")
                                            .Type("float_mat")
                                            .Shape(dim, batch_size)
                                            .Finalize())
                                 .Finalize();

    auto node_def_1 =
        NodeDefBuilder()
            .Name("node-1")
            .Signature("identical_func:11")
            .Attr("dim", AttrValueBuilder().Value(dim).Finalize())
            .Attr("batch_size", AttrValueBuilder().Value(batch_size).Finalize())
            .Finalize();
    auto node_def_2 =
        NodeDefBuilder()
            .Name("node-2")
            .Signature("identical_func:11")
            .Attr("dim", AttrValueBuilder().Value(dim).Finalize())
            .Attr("batch_size", AttrValueBuilder().Value(batch_size).Finalize())
            .Finalize();
    auto node_def_3 =
        NodeDefBuilder()
            .Name("node-3")
            .SetIsEnd(false)
            .Signature("identical_func:11")
            .Attr("dim", AttrValueBuilder().Value(dim).Finalize())
            .Attr("batch_size", AttrValueBuilder().Value(batch_size).Finalize())
            .Finalize();

    auto loss_node_def =
        NodeDefBuilder()
            .Name("loss")
            .SetIsEnd(true)
            .Signature("rmse_loss_func:11")
            .Attr("dim", AttrValueBuilder().Value(dim).Finalize())
            .Attr("batch_size", AttrValueBuilder().Value(batch_size).Finalize())
            .Finalize();

    auto edge_def_1 = EdgeDefBuilder()
                          .SrcNode("node-1")
                          .SrcArg("output")
                          .TrgNode("node-2")
                          .TrgArg("input")
                          .Finalize();

    auto edge_def_2 = EdgeDefBuilder()
                          .SrcNode("node-2")
                          .SrcArg("output")
                          .TrgNode("node-3")
                          .TrgArg("input")
                          .Finalize();

    auto edge_def_3 = EdgeDefBuilder()
                          .SrcNode("data-provider")
                          .SrcArg("input")
                          .TrgNode("node-1")
                          .TrgArg("input")
                          .Finalize();

    auto edge_def_4 = EdgeDefBuilder()
                          .SrcNode("node-3")
                          .SrcArg("output")
                          .TrgNode("loss")
                          .TrgArg("prediction")
                          .Finalize();
    auto edge_def_5 = EdgeDefBuilder()
                          .SrcNode("data-provider")
                          .SrcArg("label")
                          .TrgNode("loss")
                          .TrgArg("target")
                          .Finalize();

    graph_def = graph_builder.AddNode(node_def_1)
                    .SetDataProvider(data_provider_def)
                    .AddNode(loss_node_def)
                    .AddNode(node_def_2)
                    .AddNode(node_def_3)
                    .AddEdge(edge_def_1)
                    .AddEdge(edge_def_2)
                    .AddEdge(edge_def_3)
                    .AddEdge(edge_def_4)
                    .AddEdge(edge_def_5)
                    .Finalize();
  }

  void CreateGraph() {
    session = Session::Create("demo_session");
    session->CreateGraph(graph_def);
  }

  void SetUp() {
    CreateGraphDef();
    CreateGraph();
  }

protected:
  GraphDef graph_def;
  std::unique_ptr<Session> session;
  int64_t dim = 20;
  int64_t batch_size = 30;
};

TEST_F(SessionTest, create_graph) {
  LOG(INFO) << session->postbox().DebugString();
}

TEST_F(SessionTest, data_provider) {
  // session->StartExec();
  DLOG(INFO) << "after exec, still can run something";

  // Data provider fill data batch
  ArgumentDef batch_def = ArgumentDefBuilder()
                              .Name("data")
                              .Type("float_mat")
                              .Shape(dim, batch_size)
                              .Doc("a batch of data")
                              .Finalize();
  Argument data(&batch_def);
  std::vector<ArgumentDef> args;
  args.push_back(batch_def);
  data.Serialize(&args.back());
  DLOG(INFO) << "send batch of data to data provider";

  args.push_back(batch_def);
  data.Serialize(&args.back());

  ASSERT_TRUE(session->Compute(args).ok());

  // session->DestroyGraph();
}

} // namespace test
} // namespace chasky
