#include "common.h"

typedef unsigned short u16;
typedef char *va_list;
char *func_80048480(u16 id);
void func_80054868(char *fmt, void *args);
void func_800483B8(u16 id, ...) {
    va_list args;
    args = (va_list)__builtin_next_arg(id);
    func_80054868(func_80048480(id), args);
}
