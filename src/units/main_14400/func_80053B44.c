#include "common.h"

/* Varargs formatter front end: the sixth fixed argument is the format string
 * (func_80053B74 forwards it to the func_8005ECF8 parser, which reads it with
 * lbu); the trailing pointer is the argument cursor after the format.
 * The delegate's signed status is intentionally discarded by this void front end.
 */
s32 func_80053B74(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, const char *fmt, void *args);

void func_80053B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, const char *fmt, ...)
{
    func_80053B74(a0, a1, a2, a3, a4, fmt, __builtin_next_arg(fmt));
}
