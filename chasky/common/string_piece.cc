#include <cstring>
#include "chasky/common/string_piece.h"
using namespace std;
using namespace chasky;

StringPiece StringPiece::substr(size_t start, size_t len) const {
  CHECK_LT(start, size());
  CHECK_LT(start + len, size());
  return StringPiece(start_ + start, len);
}

string StringPiece::tostring() const {
  string res(start_, size());
  return res;
}

vector<StringPiece> StringPiece::split(const char *delimiter,
                                       bool skip_empty) const {
  vector<StringPiece> res;
  const size_t delim_len = strlen(delimiter);
  size_t len = 0;
  if (size() <= delim_len)
    return res;
  iterator start_pos = start_;
  iterator end_pos = end();
  for (iterator it = begin(); it < end(); it++) {
    if (0 == memcmp(it, delimiter, delim_len)) {
      end_pos = it;
      if (end_pos != end()) {
        res.emplace_back(start_pos, end_pos - start_pos);
      }
      it += delim_len;
      start_pos = it;
      end_pos = end();
    }
  }

  len = end_pos - start_pos;
  if (start_pos <= end()) {
    if (!skip_empty || len) {
      res.emplace_back(start_pos, len);
    }
  }
  return res;
}

bool StringPiece::starts_with(StringPiece x) const {
  if (x.size() > size())
    return false;
  return 0 == memcmp(x.data(), data(), x.size());
}

bool StringPiece::ends_with(StringPiece x) const {
  if (x.size() > size())
    return false;
  return 0 == memcmp(x.data(), data() + size() - x.size(), x.size());
}
