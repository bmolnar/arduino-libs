#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include "xerr.h"
#include "xfmt.h"
#include "xstream.h"
#include "xprint.h"

static inline double
xprintf_rounding(uint8_t digit)
{
    switch(digit) {
    case 0:
        return 0.5;
    case 1:
        return 0.05;
    case 2:
        return 0.005;
    case 3:
        return 0.0005;
    case 4:
        return 0.00005;
    case 5:
        return 0.000005;
    case 6:
        return 0.0000005;
    case 7:
        return 0.00000005;
    default:
        return 0.000000005;
    }
}
static inline unsigned long
xprintf_pow10ul(uint8_t pow)
{
    switch(pow) {
    case 0:
        return 1;
    case 1:
        return 10;
    case 2:
        return 100;
    case 3:
        return 1000;
    case 4:
        return 10000;
    case 5:
        return 100000;
    case 6:
        return 1000000;
    case 7:
        return 10000000;
    default:
        return 100000000;
    }
}
static int
xprintf_printchar(xstream_t *xp, char c)
{
    return (int) xstream_write(xp, (const void *) &c, sizeof(c));
}
static int
xprintf_printstrn(xstream_t *xp, const char *str, size_t len)
{
    return (int) xstream_write(xp, (const void *) str, len);
}
static int
xprintf_printstrp(xstream_t *xp, const char *str, const char *end)
{
  size_t len = (end > str) ? (end - str) : 0;
  return (int) xstream_write(xp, (const void *) str, len);
}
static int
xprintf_printstr(xstream_t *xp, const char *str)
{
    return (int) xstream_write(xp, (const void *) str, (size_t) strlen(str));
}




static int
xprintf_printcarg(xstream_t *xp, xfmt_carg_t arg, const struct xfmt_spec *fmtspec)
{
    ((void) fmtspec);
    return (int) xprintf_printchar(xp, (char) arg);
}
static int
xprintf_printsarg(xstream_t *xp, xfmt_sarg_t arg, const struct xfmt_spec *fmtspec)
{
    ((void) fmtspec);
    return (int) xprintf_printstr(xp, (const char *) arg);
}
static int
xprintf_printnum(xstream_t *xp, xfmt_unum_t num, int8_t sgn, const struct xfmt_spec *fmtspec)
{
    xfmt_base_t base = xfmt_spec_base(fmtspec);
    uint8_t radix = xfmt_base_radix(base);
    char buf[3*sizeof(num) + 1];
    char *endp = &buf[sizeof(buf) - 1];
    char *strp = endp;
    uint8_t dosign = (sgn < 0 || (fmtspec->flags & XFMT_FLAG_PLUS));
    uint8_t leftspace = 0, padlen = 0, numlen = 0, rightspace = 0;
    int len = 0;

    *strp = '\0';
    do
    {
        *(--strp) = xfmt_base_digitchar(base, (uint8_t)(num % radix));
        num /= radix;
    }
    while (num > 0);


    numlen = (uint8_t)(endp - strp) + (dosign ? 1 : 0);
    if (fmtspec->width > numlen)
    {
        if (fmtspec->flags & XFMT_FLAG_ZEROPAD)
        {
            padlen = fmtspec->width - numlen;
            leftspace = 0;
            rightspace = 0;
        }
        else if (fmtspec->flags & XFMT_FLAG_LEFTADJ)
        {
            padlen = 0;
            leftspace = 0;
            rightspace = fmtspec->width - numlen;
        }
        else
        {
            padlen = 0;
            leftspace = fmtspec->width - numlen;
            rightspace = 0;
        }
    }


    while (leftspace-- > 0)
    {
        len += xprintf_printchar(xp, ' ');
    }
    if (dosign)
    {
        len += xprintf_printchar(xp, ((sgn > 0) ? '+' : '-'));
    }
    while (padlen-- > 0)
    {
        len += xprintf_printchar(xp, '0');
    }
    if (endp > strp)
    {
        len += xprintf_printstrp(xp, strp, endp);
    }
    while (rightspace-- > 0)
    {
        len += xprintf_printchar(xp, ' ');
    }
    return len;
}
static int
xprintf_printfarg(xstream_t *xp, xfmt_farg_t num, const struct xfmt_spec *fmtspec)
{
    uint8_t prec = fmtspec->prec;
    int8_t sgn = (num >= 0.0f) ? 1 : -1;
    unsigned long pow, num_d, num_r;
    uint8_t m;
    int len = 0;

    if (num < 0.0)
    {
        num = -num;
    }

    num += xprintf_rounding(prec);
    pow = xprintf_pow10ul(prec);
    num_d = (unsigned long) num;
    num_r = ((unsigned long)(num * ((double) pow))) % pow;

    len += xprintf_printnum(xp, num_d, sgn, fmtspec);
    if (prec > 0)
    {
        len += xprintf_printchar(xp, '.'); 
        while (prec-- > 0)
        {
            num_r *= 10;
            m = (uint8_t)(num_r / pow);
            len += xprintf_printchar(xp, xfmt_base_digitchar(XFMT_BASE_10, m));
            num_r -= (m * pow);
        }
    }
    return len;
}
static int
xprintf_printatarg(xstream_t *xp, const xfmt_atarg_t *arg, const struct xfmt_spec *fmtspec)
{
    xprintf_atarg_fn_t *fn = (xprintf_atarg_fn_t *) arg->fn;
    return (*fn)(xp, &arg->args);
}





struct xprintf_data {
    xstream_t *xs;
    const char *fmt;
};
#define XPRINTF_DATA_INIT(a_xs,a_fmt,a_ap) { .xs = (a_xs), .fmt = (a_fmt) }
static int
xprintf_xfmt_ops_begin(xfmt_handler_t *xf, const char *pos)
{
    struct xprintf_data *data = (struct xprintf_data *) XFMT_HANDLER_PRIV(xf);
    ((void) data);
    return 0;
}
static int
xprintf_xfmt_ops_end(xfmt_handler_t *xf, const char *pos)
{
    struct xprintf_data *data = (struct xprintf_data *) XFMT_HANDLER_PRIV(xf);
    ((void) data);
    return 0;
}
static int
xprintf_xfmt_ops_onval(xfmt_handler_t *xf, const char *pos, const char *end, const struct xfmt_spec *fmtspec, const xfmt_val_t *arg)
{
    struct xprintf_data *data = (struct xprintf_data *) XFMT_HANDLER_PRIV(xf);
    switch (fmtspec->conv) {
    case 'c':
        return xprintf_printchar(data->xs, (char) arg->v_c);
    case 'd':
    case 'i':
        return xprintf_printnum(data->xs, (xfmt_unum_t)((arg->v_inum >= 0) ? arg->v_inum : -arg->v_inum), ((arg->v_inum >= 0) ? 1 : -1), fmtspec);
    case 'o':
        return xprintf_printnum(data->xs, arg->v_unum, 1, fmtspec);
    case 'u':
        return xprintf_printnum(data->xs, arg->v_unum, 1, fmtspec);
    case 'x':
    case 'X':
        return xprintf_printnum(data->xs, arg->v_unum, 1, fmtspec);
    case 'f':
        return xprintf_printfarg(data->xs, arg->v_f, fmtspec);
    case 's':
        return xprintf_printsarg(data->xs, arg->v_s, fmtspec);
    case '@':
        return xprintf_printatarg(data->xs, &arg->v_at, fmtspec);
    case '%':
        return xprintf_printchar(data->xs, '%');
    default:
        return -XERR_INVAL;
    }
}
static int
xprintf_xfmt_ops_onchars(xfmt_handler_t *xf, const char *pos, const char *end)
{
    struct xprintf_data *data = (struct xprintf_data *) XFMT_HANDLER_PRIV(xf);
    ((void) data);
    return xprintf_printstrn(data->xs, pos, (end-pos));
}
static xfmt_handler_ops_t xprintf_xfmt_ops = {
    .op_begin     = &xprintf_xfmt_ops_begin,
    .op_end       = &xprintf_xfmt_ops_end,
    .op_onval     = &xprintf_xfmt_ops_onval,
    .op_onchars   = &xprintf_xfmt_ops_onchars,
};
int
xprintf_vxprintf(xstream_t *xs, const char *format, va_list ap)
{
    struct xprintf_data      data = XPRINTF_DATA_INIT(xs, format, ap);
    xfmt_handler_t             xf = XFMT_HANDLER_INIT(&xprintf_xfmt_ops, &data);
    return xfmt_parsev(&xf, format, ap);
}


int
xprintfv(xstream_t *xp, const char *format, va_list ap)
{
    return xprintf_vxprintf(xp, format, ap);
}
int
xprintf(xstream_t *xp, const char *format, ...)
{
    int rv;
    va_list ap;

    va_start(ap, format);
    rv = xprintf_vxprintf(xp, format, ap);
    va_end(ap);

    return rv;
}
