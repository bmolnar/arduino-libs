#include "Pin.h"

Pin pin(10, OUTPUT);

void setup(void)
{
}
void loop(void)
{
  pin.setValue(HIGH);
  delay(1000);
  pin.setValue(LOW);
  delay(1000);
}
