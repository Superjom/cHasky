#include <gtest/gtest.h>
#include "chasky/common/strings.h"
#include "chasky/core/common/register.h"
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/function.h"
#include "chasky/core/framework/node.h"
#include "chasky/core/framework/graph.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/attr_value_util.h"
namespace chasky {
namespace test {

using std::string;

TEST(Edge, GenEdgeKey) {
  string node1 = "src";
  string node2 = "trg";
  string arg1 = "arg1";
  string arg2 = "arg2";

  string signature = "src:arg1->trg:arg2";

  ASSERT_EQ(GenEdgeKey(node1, arg1, node2, arg2), signature);

  // string input = strings::Printf("%s:%s", node1.c_str(), arg.c_str());
  // ASSERT_EQ(GenEdgeKey(input, node2), signature);
}

REGISTER_FUNC_DEF(
    demo_func,
    FunctionDefBuilder()
        .Name("demo_func")
        .Input(ArgumentDefBuilder().Name("src1").Type("float_mat").Finalize())
        .Input(ArgumentDefBuilder().Name("src2").Type("float_mat").Finalize())
        .Output(
            ArgumentDefBuilder().Name("output1").Type("float_mat").Finalize())
        .Param(ArgumentDefBuilder().Name("param1").Type("float_mat").Finalize())
        .Attr(AttrDefBuilder().Name("attr1").Type("int32").Finalize())
        .Finalize());

TEST(Node, Create) {
  auto sign = Function::Signature("demo_func", CH_FLOAT);
  NodeDef def;
  def.set_name("node1");
  def.set_signature(sign);

  LOG(INFO) << "function_library:\n"
            << FunctionLibrary::Instance().DebugString();

  LOG(INFO) << "function_def_library:\n"
            << FunctionDefLibrary::Instance().DebugString();

  auto graph = Graph::Create();
  auto node = Node::Create(def, graph.get());
  ASSERT_TRUE(node != nullptr);
}

} // namespace test
} // namespace chasky
