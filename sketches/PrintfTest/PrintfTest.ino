#include "PrintfWrap.h"

int print_sel(Printf& out, unsigned long loopcnt)
{
  unsigned int idx = (unsigned int)(loopcnt % 16);

  switch (idx)
  {
    case 0:
      return out.printf("arg_d=%d, arg_f=%f, arg_s=\"%s\", arg_u=%u\n", -2, 0.3, "testing", (unsigned) 10);
    case 1:
      return out.printf("idx=%u\n", idx);
    default:
      return out.printf("idx=%u, END\n", idx);
  }
}



PrintfWrap out(Serial);
unsigned long loopcnt=0;
void setup(void)
{
  Serial.begin(38400);
}
void loop(void)
{
  print_sel(out, loopcnt);
  delay(1000);
  loopcnt++;
}