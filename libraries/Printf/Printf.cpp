#include "Printf.h"

#include <stdarg.h>
#include "xlib.h"

__XLIB_BEGIN_DECL

static int
_printf_xsop_open(xstream_t *xs, const char *mode, void *userdata)
{
    return 0;
}
static ssize_t
_printf_xsop_write(xstream_t *xs, const void *buf, size_t count)
{
    Printf *xp = (Printf *) XSTREAM_PRIV(xs);
    return (ssize_t) xp->write((const uint8_t *) buf, (size_t) count);
}
static ssize_t
_printf_xsop_read(xstream_t *xs, void *buf, size_t count)
{
    return (ssize_t) 0;
}
static int
_printf_xsop_flush(xstream_t *xs)
{
    return 0;
}
static int
_printf_xsop_close(xstream_t *xs)
{
    return 0;
}
static xstream_ops_t _printf_xsops = {
    .open  = &_printf_xsop_open,
    .write = &_printf_xsop_write,
    .read  = &_printf_xsop_read,
    .flush = &_printf_xsop_flush,
    .close = &_printf_xsop_close,
};

__XLIB_END_DECL

size_t Printf::printfv(const char *fmt, va_list ap)
{
  xstream_t xs = XSTREAM_INIT(_printf_xsops, (void *) this);
  int rv = xprintfv(&xs, fmt, ap);
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
