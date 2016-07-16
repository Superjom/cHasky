#include <sstream>

#include "gtest/gtest.h"
#include "chasky/common/strings.h"

namespace chasky {
namespace test{
using namespace std;

TEST(Printf, printf) {
  // test with small buffer
  string res = strings::Printf("hello %d, %s", 2008, "Joms");
  ASSERT_EQ(res, "hello 2008, Joms");
  // test with large buffer
  string format_prefix(200, '_');
  string format = format_prefix + "%d,%d,%s,%.1f";
  string res1 = strings::Printf(format.c_str(), 1, 2, "hello", 0.7);
  stringstream ss;
  ss << format_prefix;
  ss << 1 << "," << 2 << ","
     << "hello," << 0.7;
  ASSERT_EQ(res1, ss.str());
}

TEST(Appendf, appendf) {
  string res = "hello";
  strings::Appendf(&res, "%d%.1f", 2008, 0.7);
  ASSERT_EQ(res, "hello20080.7");
}

TEST(Split, split) {
  StringPiece a("hello world");
  auto pieces = strings::Split(a, ' ');
  ASSERT_EQ(pieces.size(), 2);
  ASSERT_EQ(pieces[0], "hello");
  ASSERT_EQ(pieces[1], "world");
}

}; // namespace test
}; // namespace chasky
