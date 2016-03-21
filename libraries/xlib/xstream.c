#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

#include "xerr.h"
#include "xstream.h"

static int
xstream_do_open(xstream_t *xs, const char *mode, void *userdata)
{
    if (xs->xs_ops.open == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*xs->xs_ops.open)(xs, mode, userdata);
}
static int
xstream_do_close(xstream_t *xs)
{
    if (xs->xs_ops.close == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*xs->xs_ops.close)(xs);
}
static ssize_t
xstream_do_write(xstream_t *xs, const void *buf, size_t sz)
{
    if (xs->xs_ops.write == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*xs->xs_ops.write)(xs, buf, sz);
}
static ssize_t
xstream_do_read(xstream_t *xs, void *buf, size_t sz)
{
    if (xs->xs_ops.read == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*xs->xs_ops.read)(xs, buf, sz);
}
static int
xstream_do_flush(xstream_t *xs)
{
    if (xs->xs_ops.flush == NULL)
    {
        return -XERR_NOSYS;
    }
    return (*xs->xs_ops.flush)(xs);
}

static xstream_t *
xstream_alloc()
{
    return (xstream_t *) malloc(sizeof(xstream_t));
}
void
xstream_free(xstream_t *xs)
{
    free(xs);
}


/*
 * public interface
 */

int
xstream_init(xstream_t *xs, xstream_ops_t ops)
{
    xs->xs_ops = ops;
    xs->xs_priv = NULL;
    xs->xs_hnd = NULL;
    return 0;
}
void
xstream_cleanup(xstream_t *xs)
{
}
xstream_t *
xstream_create(xstream_ops_t ops)
{
    xstream_t *xs;
    int rv = 0;

    xs = xstream_alloc(sizeof(*xs));
    if (xs == NULL)
    {
        errno = XERR_NOMEM;
        return NULL;
    }
    rv = xstream_init(xs, ops);
    if (rv < 0)
    {
        xstream_free(xs);
        errno = -rv;
        return NULL;
    }
    return xs;
}
void
xstream_destroy(xstream_t *xs)
{
    xstream_cleanup(xs);
    xstream_free(xs);
}


int
xstream_open(xstream_t *xs, const char *mode, void *userdata)
{
    int rv;

    rv = xstream_do_open(xs, mode, userdata);
    if (rv < 0 && rv != -XERR_NOSYS)
    {
        return rv;
    }
    return 0;
}
int
xstream_close(xstream_t *xs)
{
    int rv;

    rv = xstream_do_close(xs);
    if (rv < 0 && rv != -XERR_NOSYS)
    {
        return rv;
    }
    return 0;
}
ssize_t
xstream_write(xstream_t *xs, const void *buf, size_t sz)
{
    return xstream_do_write(xs, buf, sz);
}
ssize_t
xstream_read(xstream_t *xs, void *buf, size_t sz)
{
    return xstream_do_read(xs, buf, sz);
}
int
xstream_flush(xstream_t *xs)
{
    return xstream_do_flush(xs);
}
xstream_t *
xstream_create_open(xstream_ops_t ops, const char *mode, void *userdata)
{
    xstream_t *xs;
    int rv;

    xs = xstream_create(ops);
    if (xs == NULL)
    {
        return xs;
    }
    rv = xstream_open(xs, mode, userdata);
    if (rv < 0 && rv != -XERR_NOSYS)
    {
        xstream_free(xs);
        errno = -rv;
        return NULL;
    }
    return xs;
}
int
xstream_close_destroy(xstream_t *xs)
{
    int rv;

    rv = xstream_close(xs);
    xstream_destroy(xs);
    return rv;
}
