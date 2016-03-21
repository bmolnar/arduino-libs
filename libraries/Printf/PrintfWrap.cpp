#include "PrintfWrap.h"

PrintfWrap::PrintfWrap(Print& inst)
  : inst_(inst)
{
}
size_t PrintfWrap::write(uint8_t wbyte)
{
  return this->inst_.write(wbyte);
}
size_t PrintfWrap::write(const uint8_t *buffer, size_t size)
{
  return this->inst_.write(buffer, size);
}
