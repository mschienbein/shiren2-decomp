#include "common.h"
typedef char *va_list;
extern char D_801A89A0[];
s32 func_8005ECF8(char *dst, const char *fmt, va_list args);
void func_80082A20(char *);
void func_800829D4(const char *fmt, ...) {
    va_list args;
    args = (va_list)__builtin_next_arg(fmt);
    func_8005ECF8(D_801A89A0, fmt, args);
    func_80082A20(D_801A89A0);
}
