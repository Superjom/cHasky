#include <gtest/gtest.h>
#include "chasky/common/strings.h"
#include "chasky/core/framework/node.h"
namespace chasky {
namespace test {

using std::string;

TEST(Edge, GenEdgeKey) {
  string node1 = "src";
  string node2 = "trg";
  string arg = "arg1";

  string signature = "src:arg1->trg";

  ASSERT_EQ(GenEdgeKey(node1, node2, arg), signature);

  string input = strings::Printf("%s:%s", node1.c_str(), arg.c_str());
  ASSERT_EQ(GenEdgeKey(input, node2), signature);
}



} // namespace test
} // namespace chasky
