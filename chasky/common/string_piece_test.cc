#include <gtest/gtest.h>
#include "chasky/common/string_piece.h"
using namespace std;
using namespace chasky;

TEST(StringPiece, init) {
  StringPiece piece;
  ASSERT_TRUE(piece.empty());

  string str = "Superjom hello world";
  StringPiece piece1(str);
  StringPiece piece2(str, str.size());
  StringPiece piece3(str.data(), str.size());
  ASSERT_TRUE(piece1 == piece2 && piece3 == piece1);
}

TEST(StringPiece, substr) {
  StringPiece piece("hello", 5);
  StringPiece substr1 = piece.substr(1, 3);
  cout << "substr1\t" << substr1 << endl;
  ASSERT_TRUE(substr1 == string("ell"));
}

TEST(StringPiece, tostring) {
  StringPiece piece("hello", 3);
  ASSERT_EQ(piece.tostring(), "hel");
}

TEST(StringPiece, clear) {
  StringPiece piece("hello");
  piece.clear();
  ASSERT_EQ(piece.size(), 0);
}

class TestStringPiece : public ::testing::Test {
protected:
  virtual void SetUp() {
    str = "helloworld";
    piece.set(str.data(), str.size());
  }

protected:
  string str;
  StringPiece piece;
};

TEST_F(TestStringPiece, remove_prefix) {
  piece.remove_prefix(5);
  ASSERT_EQ(piece, string("world"));
}

TEST_F(TestStringPiece, remove_suffix) {
  piece.remove_suffix(5);
  ASSERT_EQ(piece, string("hello"));
}

TEST_F(TestStringPiece, starts_with) {
  string str = "hello";
  ASSERT_TRUE(piece.starts_with(str));

  string str2 = "world";
  ASSERT_TRUE(piece.ends_with(str2));

  string str3 = "es";
  ASSERT_FALSE(piece.ends_with(str3));
}

TEST(StringPiece, split) {
  string str = "hello world";
  StringPiece piece(str);
  auto parts = piece.split(" ");
  ASSERT_EQ(parts.size(), 2);
  ASSERT_EQ(parts[0], "hello");
  ASSERT_EQ(parts[1], "world");
}
TEST(StringPiece, split1) {
  string str = "hello@@*7f@@workd1@@";
  vector<string> str_parts = {"hello", "*7f", "workd1"};
  StringPiece piece(str);
  auto parts = piece.split("@@");
  ASSERT_EQ(parts.size(), str_parts.size());
  for (size_t i = 0; i < parts.size(); i++) {
    LOG(INFO) << i << "\t" << parts[i];
    ASSERT_EQ(parts[i], str_parts[i]);
  }
}
