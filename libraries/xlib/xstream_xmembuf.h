#ifndef _XSTREAM_XMEMBUF_H
#define _XSTREAM_XMEMBUF_H

#include "xdefs.h"
#include "xstream.h"
#include "xmembuf.h"

__XLIB_BEGIN_DECL

extern xstream_ops_t xstream_xmembuf_ops;
#define XSTREAM_XMEMBUF_INIT(priv) XSTREAM_INIT(xstream_xmembuf_ops,(priv))

int xstream_xmembuf_open(xstream_t *xs, const char *mode, struct xmembuf *mbuf);
xstream_t *xstream_xmembuf_create_open(const char *mode, struct xmembuf *mbuf);

__XLIB_END_DECL

#endif /* _XSTREAM_XMEMBUF_H */
