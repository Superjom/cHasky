#ifndef CHASKY_COMMON_STRINGS_H_
#define CHASKY_COMMON_STRINGS_H_
#include <stdarg.h>
#include <string>

#include "chasky/common/macros.h"

namespace chasky {
namespace strings {

std::string Printf(const char *format, ...) CH_PRINTF_ATTRIBUTE(1, 2);

void Appendf(std::string *dst, const char *format, ...)
    CH_PRINTF_ATTRIBUTE(2, 3);

void Appendv(std::string *dst, const char *format, va_list ap);

}; // namespace strings
}; // namespace chasky
#endif
