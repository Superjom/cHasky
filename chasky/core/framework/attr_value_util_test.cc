#include <gtest/gtest.h>
#include "chasky/common/util.h"
#include "chasky/core/framework/attr_value_util.h"
using namespace std;
using namespace chasky;

TEST(AttrDefBuilder, init) {
  AttrDefBuilder builder("attr1");
  builder.Type("float").Doc("this is an attribute").Value("3.14159");
  const auto& def = builder.Def();
  ASSERT_EQ(def.name(), "attr1");
  ASSERT_EQ(def.type(), "float");
  float gap = def.value().f() - 3.14159;
  ASSERT_TRUE(chasky::abs((float)gap) < 0.000001);
  ASSERT_EQ(def.description(), "this is an attribute");
}
