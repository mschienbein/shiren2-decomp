#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

char *func_80048480(u16 id);
void func_80054740(s32 mode, char *fmt, void *args);
void func_80048294(s32 arg0, u16 arg1, ...) {
    char *ap = (char *)__builtin_next_arg(arg1);
    func_80054740(arg0, func_80048480(arg1), ap);
}
