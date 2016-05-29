#include <iostream>
#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#define CH_ENABLE_TIMER
#include "chasky/common/stat.h"

namespace chasky {
namespace test {

TEST(StatItem, Add) {
  StatItem item;
  item.AddNew(1000);

  ASSERT_EQ(item.total, 1000);
  ASSERT_EQ(item.count, 1);
  ASSERT_EQ(item.max, 1000);
  ASSERT_EQ(item.min, 1000);

  item.AddNew(10);
  ASSERT_EQ(item.total, 1010);
  ASSERT_EQ(item.min, 10);
  ASSERT_EQ(item.max, 1000);
  ASSERT_EQ(item.count, 2);
}

TEST(StateItem, DebugString) {
  StatItem item;
  item.AddNew(100);
  item.AddNew(2);
  item.AddNew(23);
  // std::cout << item.DebugString();
  std::string target1 =
      "mean:     41.667\tmax:   100\tmin:   2\tcount: 3\ttotal: 125";
  ASSERT_EQ(item.DebugString(), target1);
}

TEST(StatRegistry, Register) {
  StatRegistry regi;
  ASSERT_TRUE(regi.Register("key1") != nullptr);
  // duplicate key
  ASSERT_FALSE(regi.Register("key1") != nullptr);
}

TEST(StatRegistry, LookUp) {
  StatRegistry regi;
  ASSERT_TRUE(regi.Register("key1") != nullptr);
  auto item = regi.LookUp("key1");
  ASSERT_TRUE(item != nullptr);
  item->AddNew(1000);
  ASSERT_EQ(item->total, 1000);
  ASSERT_EQ(item->count, 1);
}

TEST(StatRegistry, LookupOrRegister) {
  StatRegistry regi;
  auto item1 = regi.LookupOrRegister("key1");
  auto item2 = regi.LookupOrRegister("key1");
  ASSERT_EQ(item1, item2);
}

  TEST(StatRegistry, DebugString) {
    StatRegistry regi;
    regi.LookupOrRegister("key1");
    regi.LookupOrRegister("key2");
    std::cout << regi.DebugString() << std::endl;
  }

// just invoke the macro to validate its effectiveness
TEST(REGISTER_TIMER, use) {
  #define CH_ENABLE_TIMER
  {
    REGISTER_TIMER("key1");
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
  }
  {
    REGISTER_TIMER("key2");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  {
    REGISTER_TIMER("key2");
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  std::cout << StatRegistry::Instance().DebugString();
}

}; // namespace test
}; // namespace chasky

#undef CH_ENABLE_TIMER
