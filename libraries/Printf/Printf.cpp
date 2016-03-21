#include <stdarg.h>
#include "xlib.h"
#include "xfmt.h"
#include "xprint.h"

#include "Printf.h"

__XLIB_BEGIN_DECL

typedef int (_printf_atarg_fn_t)(Printf *, const void *);

static int
_printf_op_write(xprint_t *xp, const void *buf, size_t len)
{
  Printf *pf = (Printf *) XPRINT_PRIV(xp);
  return (int) pf->write((const uint8_t *) buf, len);
}
static int
_printf_op_atwrap(xprint_t *xp, const xfmt_atarg_fn_t *fn, const void *ptr)
{
  Printf *pf = (Printf *) XPRINT_PRIV(xp);
  _printf_atarg_fn_t *myfn = (_printf_atarg_fn_t *) fn;
  return (*myfn)(pf, ptr);
}
static xprint_ops_t _printf_xpops = {
  .write = &_printf_op_write,
  .atwrap = &_printf_op_atwrap,
};

__XLIB_END_DECL


size_t Printf::printfv(const char *fmt, va_list ap)
{
  xprint_t xp = XPRINT_INIT(_printf_xpops, (void *) this);

  int rv = xprintfv(&xp, fmt, ap);
  return (size_t) rv;
}
size_t Printf::printf(const char *fmt, ...)
{
  size_t rv;
  va_list ap;

  va_start(ap, fmt);
  rv = this->printfv(fmt, ap);
  va_end(ap);

  return rv;
}
