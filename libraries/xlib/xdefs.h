#ifndef _XDEFS_H
#define _XDEFS_H

#ifdef __cplusplus
# define __XLIB_BEGIN_DECL extern "C" {
# define __XLIB_END_DECL }
#else
# define __XLIB_BEGIN_DECL
# define __XLIB_END_DECL 
#endif

#define __XLIB_STRINGIFY(a)    #a
#define XLIB_STRINGIFY(a)      __XLIB_STRINGIFY(a)
#define __XLIB_CONCAT2(a,b)    a ## b
#define XLIB_CONCAT2(a,b)      __XLIB_CONCAT2(a,b)
#define __XLIB_CONCAT(a,...)    a ## __VA_ARGS__
#define XLIB_CONCAT(a,...)     __XLIB_CONCAT(a, __VA_ARGS__)

#define XLIB_ARGS_GET1(arg1, ...) arg1
#define XLIB_ARGS_GET2(arg1, arg2, ...) arg2
#define XLIB_ARGS_GET3(arg1, arg2, arg3, ...) arg3
#define XLIB_ARGS_GET4(arg1, arg2, arg3, arg4, ...) arg4
#define XLIB_ARGS_GET5(arg1, arg2, arg3, arg4, arg5, ...) arg5
#define XLIB_ARGS_GET6(arg1, arg2, arg3, arg4, arg5, arg6, ...) arg6
#define XLIB_ARGS_GET7(arg1, arg2, arg3, arg4, arg5, arg6, arg7, ...) arg7
#define XLIB_ARGS_GET8(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ...) arg8
#define XLIB_ARGS_GET9(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ...) arg9
#define XLIB_ARGS_GET10(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, ...) arg10

#define XLIB_MACRO_CALL(name,...)    name ( __VA_ARGS__)

#endif /* _XDEFS_H */
