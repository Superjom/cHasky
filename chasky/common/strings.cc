// borrowed much code from google/tensorflow project.

#include "chasky/common/strings.h"

namespace chasky {
namespace strings {

void Appendv(std::string *dst, const char *format, va_list ap) {
  // first to try with a small fixed size buffer
  static const int kSpaceLength = 1024;
  char space[kSpaceLength];
  va_list ap_bk;
  // backup ap to protect ap's content, may need op's content more than once
  va_copy(ap_bk, ap);
  int result = vsnprintf(space, kSpaceLength, format, ap_bk);
  va_end(ap_bk);

  if (result < kSpaceLength) {
    if (result >= 0) {
      // Normal case
      dst->append(space, result);
    }
    return;
  }
  // Increase the buffer size to fit
  // plus one for the \0 in C str
  const int length = result + 1;
  char *buf = new char[length];

  va_copy(ap_bk, ap);
  result = vsnprintf(buf, length, format, ap_bk);
  va_end(ap_bk);

  if (result >= 0 && result < length) {
    dst->append(buf, result);
  }
  delete[] buf;
}

std::string Printf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  std::string result;
  Appendv(&result, format, ap);
  va_end(ap);
  return result;
}

void Appendf(std::string *dst, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  Appendv(dst, format, ap);
  va_end(ap);
}

} // namespace strings
} // namespace chasky
