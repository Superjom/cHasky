#include <gtest/gtest.h>
#include "chasky/common/util.h"
#include "chasky/core/framework/attr_value_util.h"
using namespace std;
using namespace chasky;

class TestAttrValueBuilder : public ::testing::Test {
 protected:
  void SetUp() {
    builder.Name("attr1").Doc("this is an attribute");
  }

  AttrDefBuilder builder;
};

TEST_F(TestAttrValueBuilder, init) {
  builder.Type("float").Value("3.14159");
  const auto &def = builder.Def();
  ASSERT_EQ(def.name(), "attr1");
  ASSERT_EQ(def.type(), "float");
  float gap = def.value().float_val() - 3.14159;
  ASSERT_TRUE(chasky::abs((float)gap) < 0.000001);
  ASSERT_EQ(def.doc(), "this is an attribute");
}

TEST_F(TestAttrValueBuilder, int64) {
  builder.Type("int64")
      .Value("2008");
  ASSERT_EQ(builder.Def().value().int64_val(), 2008);
}
TEST_F(TestAttrValueBuilder, dtype) {
  builder.Type("dtype")
      .Value("string");
  ASSERT_EQ(builder.Def().value().dtype_val(), CH_STRING);
}

TEST(AttrDefBuilder, NewAttrDefBuilder) {
  NewAttrDefBuilder().Name("hello");
}
