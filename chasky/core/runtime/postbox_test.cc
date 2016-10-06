#include <thread>
#include <gtest/gtest.h>
#include <chrono>
#include "chasky/core/runtime/postbox.h"
namespace chasky {
namespace test {
class PostBoxTest : public ::testing::Test {
public:
  void SetUp() {
    postbox.Register("node1:arg1:0");
    postbox.Register("node2:arg1:0");
    postbox.Register("node3:arg1:0");
  }

protected:
  PostBox postbox;
};

TEST_F(PostBoxTest, init) {}

TEST_F(PostBoxTest, Send) { postbox.Send("node1:arg1:0", nullptr); }

TEST_F(PostBoxTest, Consume) {
  bool flag = false;
  PostBox::ReadyCallback callback = [&](ArgumentPtr arg) {
    LOG(INFO) << "node1:arg1 callback is called";
    flag = true;
  };
  postbox.Consume("node1:arg1:0", std::move(callback));

  postbox.Send("node1:arg1:0", nullptr);

  ASSERT_TRUE(flag);
}
}
}
