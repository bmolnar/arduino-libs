#ifndef _XPRINT_H
#define _XPRINT_H

#include <stdarg.h>

#include "xfmt.h"
#include "xstream.h"

__XLIB_BEGIN_DECL

typedef int (xprintf_atarg_fn_t)(xstream_t *, const xfmt_atarg_arglist_t *);
int xprintfv(xstream_t *xp, const char *format, va_list ap);
int xprintf(xstream_t *xp, const char *format, ...);

__XLIB_END_DECL

#endif /* _XPRINT_H */
