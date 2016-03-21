#ifndef PRINTF_H_
#define PRINTF_H_

#include <stdarg.h>

#include "Print.h"

class Printf : public Print
{
 public:
  size_t printfv(const char *fmt, va_list ap);
  size_t printf(const char *fmt, ...);
};

#endif // PRINTF_H_
