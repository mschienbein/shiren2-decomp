#include "common.h"

typedef unsigned char u8;
typedef char *va_list;
extern u8 D_80161B44;
extern va_list D_80161720;
s32 func_8005ECF8(char *dst, const char *fmt, va_list args);
void func_8005457C(char*);
void func_800546F4(const char *fmt, ...){
    char buf[0x200];
    va_list args;
    args = (va_list)__builtin_next_arg(fmt);
    D_80161B44 = 0;
    D_80161720 = args;
    func_8005ECF8(buf, fmt, args);
    func_8005457C(buf);
}
