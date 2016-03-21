#ifndef PRINTF_WRAP_H_
#define PRINTF_WRAP_H_

#include <Print.h>

#include "Printf.h"

class PrintfWrap : public Printf
{
private:
  Print& inst_;
public:
  PrintfWrap(Print& inst);
  size_t write(uint8_t wbyte);
  size_t write(const uint8_t *buffer, size_t size);
};

#endif // PRINTF_WRAP_H_
