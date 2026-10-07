#include "common.h"

typedef char *va_list;

extern s32 func_8005ECA8(char *dst, const char *fmt, va_list args);

s32 func_8005EF30(char *dst, const char *fmt, ...) {
    return func_8005ECA8(dst, fmt, (va_list)__builtin_next_arg(fmt));
}
