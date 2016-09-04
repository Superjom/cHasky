#include <thread>
#include <gtest/gtest.h>
#include <chrono>
#include "chasky/core/runtime/postbox.h"
namespace chasky {
namespace test {
class PostBoxTest : public ::testing::Test {
public:
  void SetUp() {
    postbox.Register("node1:arg1");
    postbox.Register("node2:arg1");
    postbox.Register("node3:arg1");
  }

protected:
  PostBox postbox;
};

TEST_F(PostBoxTest, init) {}

TEST_F(PostBoxTest, Send) { postbox.Send("node2:arg1->node1:arg1", nullptr); }

TEST_F(PostBoxTest, Consume) {
  bool flag = false;
  PostBox::ReadyCallback callback = [&](Argument *arg) {
    LOG(INFO) << "node1:arg1 callback is called";
    flag = true;
  };
  postbox.Consume("node1:arg1", std::move(callback));

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  postbox.Send("node2:arg1->node1:arg1", nullptr);

  ASSERT_TRUE(flag);
}

}
}
