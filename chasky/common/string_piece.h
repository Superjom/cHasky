#ifndef CHASKY_COMMON_STRING_PIECE_H_
#define CHASKY_COMMON_STRING_PIECE_H_
#include <iostream>
#include <vector>
#include <string>
#include "glog/logging.h"
namespace chasky {
// This is a helper class to make visting string parts much cheaper. We borrowed
// much ideas from google/tensorflow::StringPiece
// NOTE StringPiece will never allocate memory to store string's content, so
// much attention should be paid to mentain the original string's life.
class StringPiece {
public:
  StringPiece() : start_(nullptr), len_(0) {}

  StringPiece(const StringPiece &other)
      : start_(other.data()), len_(other.size()) {}

  // NOTE attention with the life circle of 'str'
  // StringPiece(const char *str) : start_(str), len_(strlen(str)) {}

  StringPiece(const std::string &other)
      : start_(other.data()), len_(other.size()) {}

  StringPiece(const std::string &other, size_t len)
      : start_(other.data()), len_(len) {}

  StringPiece(const char *data, size_t len) : start_(data), len_(len) {}

  void set(const char *start, size_t len) {
    start_ = start;
    len_ = len;
  }

  const char *data() const { return start_; }

  const char *c_str() const { return data(); }

  size_t size() const { return len_; }

  StringPiece substr(size_t start, size_t len) const;

  std::string tostring() const;

  // Like python's str.split()
  std::vector<StringPiece> split(const char *delimiter,
                                 bool skip_empty = true) const;

  bool empty() const { return len_ == 0; }

  typedef const char *iterator;

  iterator begin() const { return start_; }
  iterator end() const { return start_ + len_; }

  char operator[](size_t i) const {
    CHECK_LT(i, size());
    return start_[i];
  }

  void clear() {
    start_ = "";
    len_ = 0;
  }

  void remove_prefix(size_t n) {
    CHECK_LT(n, size());
    start_ += n;
    len_ -= n;
  }

  void remove_suffix(size_t n) {
    CHECK_LT(n, size());
    len_ -= n;
  }

  bool starts_with(StringPiece x) const;
  bool ends_with(StringPiece x) const;

private:
  const char *start_;
  // just like std::vector::end(), point to the next byte of StringPiece's last
  // byte;
  size_t len_;
};

inline bool operator==(const StringPiece &a, const StringPiece &b) {
  return a.data() == b.data() && a.size() == b.size();
}

inline bool operator!=(const StringPiece &a, const StringPiece &b) {
  return !(a == b);
}

inline bool operator==(const StringPiece &a, const std::string &b) {
  return a.size() == b.size() && 0 == memcmp(a.data(), b.data(), a.size());
}

inline bool operator!=(const StringPiece &a, const std::string &b) {
  return !(a == b);
}

inline std::ostream &operator<<(std::ostream &os, const StringPiece &x) {
  os << x.tostring();
  return os;
}
}
#endif
