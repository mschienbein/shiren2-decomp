#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef char *va_list;
u32 func_80048AC8(void);
char *func_80048480(u16 id);
void func_800546BC(char *fmt, void *args);

void func_80048240(u16 id, ...) {
    va_list args;

    if ((func_80048AC8() ^ 1) == 0) {
        args = (va_list)__builtin_next_arg(id);
        func_800546BC(func_80048480(id), args);
    }
}
