#ifndef CHASKY_COMMON_STRINGS_H_
#define CHASKY_COMMON_STRINGS_H_
#include <stdarg.h>
#include <vector>
#include <string>

#include "chasky/common/macros.h"
#include "chasky/common/string_piece.h"

namespace chasky {
namespace strings {

static const std::string empty_string;

std::string Printf(const char *format, ...) CH_PRINTF_ATTRIBUTE(1, 2);

void Appendf(std::string *dst, const char *format, ...)
    CH_PRINTF_ATTRIBUTE(2, 3);

void Appendv(std::string *dst, const char *format, va_list ap);

std::vector<std::string> Split(StringPiece s, char delim);

}; // namespace strings
}; // namespace chasky
#endif
