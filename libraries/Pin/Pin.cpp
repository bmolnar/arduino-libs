#include <Arduino.h>

#include "Pin.h"

Pin::Pin(uint8_t num, uint8_t mode)
  : num_(num), mode_(mode)
{
  pinMode(this->num_, this->mode_);
}
uint8_t Pin::getNum()
{
  return this->num_;
}
uint8_t Pin::getMode()
{
  return this->mode_;
}
uint8_t Pin::getValue()
{
  if (this->mode_ == INPUT || this->mode_ == INPUT_PULLUP)
  {
    return digitalRead(this->num_);
  }
  else
  {
    return 0;
  }
}
bool Pin::setValue(uint8_t value)
{
  if (this->mode_ == OUTPUT)
  {
    digitalWrite(this->num_, value);
    return true;
  }
  else
  {
    return false;
  }
}
