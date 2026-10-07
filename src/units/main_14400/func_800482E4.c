#include "common.h"

typedef unsigned short u16;

extern char *func_80048480(u16 id);
extern void func_80054790(s32 mode, char *fmt, void *args);

void func_800482E4(s32 target, u16 id, ...) {
    char *args = (char *)__builtin_next_arg(id);

    func_80054790(target, func_80048480(id), args);
}
