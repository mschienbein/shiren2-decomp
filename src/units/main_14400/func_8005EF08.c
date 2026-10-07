#include "common.h"
typedef char *va_list;
s32 func_8005ECF8(char *dst, const char *fmt, va_list args);
s32 func_8005EF08(char *dst, const char *fmt, ...) { return func_8005ECF8(dst, fmt, (va_list)__builtin_next_arg(fmt)); }
