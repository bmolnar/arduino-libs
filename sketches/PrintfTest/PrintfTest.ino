#include "PrintfWrap.h"

struct myobj
{
  unsigned long a;
  unsigned long b;
  int           c;
  const char *  d;
};
#define MYOBJ_INIT(va,vb,vc,vd) { .a = (va), .b = (vb), .c = (vc), .d = (vd) }

int print_myobj(Printf& out, const struct myobj *myobj)
{
  return out.printf("{ .a=%lu, .b=%lu, .c=%d, .d=\"%s\" }", myobj->a, myobj->b, myobj->c, myobj->d);
}



int print_sel(Printf& out, unsigned long loopcnt)
{
  unsigned int idx = (unsigned int)(loopcnt % 16);
  struct myobj obj = { .a = 10, .b = 20, .c = -5, .d = "testing" };

  switch (idx)
  {
    case 0:
      return out.printf("arg_d=%d, arg_f=%.05f, arg_s=\"%s\", arg_u=%u\n", -2, 0.3, "testing", (unsigned) 10);
    case 1:
      return out.printf("idx=%u\n", idx);
    case 2:
      return out.printf("arg_s=\"%s\", arg_@=%@, arg_s=\"%s\"\n", "pre", print_myobj, &obj, "post");
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
