#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "xtypes.h"
#include "xstream.h"
#include "xmembuf.h"

static int
xstream_xmembuf_open_fn(xstream_t *xs, const char *mode, void *userdata)
{
    struct xmembuf *mbuf = (struct xmembuf *) userdata;
    XSTREAM_HND(xs) = mbuf;
    return 0;
}
static ssize_t
xstream_xmembuf_write_fn(xstream_t *xs, const void *buf, size_t count)
{
    struct xmembuf *mbuf = (struct xmembuf *) XSTREAM_HND(xs);
    return xmembuf_putbytes(mbuf, buf, count);
}
static ssize_t
xstream_xmembuf_read_fn(xstream_t *xs, void *buf, size_t count)
{
    struct xmembuf *mbuf = (struct xmembuf *) XSTREAM_HND(xs);
    return xmembuf_getbytes(mbuf, buf, count);
}
static int
xstream_xmembuf_close_fn(xstream_t *xs)
{
    return 0;
}
static int
xstream_xmembuf_flush_fn(xstream_t *xs)
{
    return 0;
}

xstream_ops_t xstream_xmembuf_ops = {
    .open  = &xstream_xmembuf_open_fn,
    .write = &xstream_xmembuf_write_fn,
    .read  = &xstream_xmembuf_read_fn,
    .close = &xstream_xmembuf_close_fn,
    .flush = &xstream_xmembuf_flush_fn,
};
int
xstream_xmembuf_open(xstream_t *xs, const char *mode, struct xmembuf *mbuf)
{
    return xstream_open(xs, mode, (void *) mbuf);
}
xstream_t *
xstream_xmembuf_create_open(const char *mode, struct xmembuf *mbuf)
{
    return xstream_create_open(xstream_xmembuf_ops, mode, mbuf);
}
