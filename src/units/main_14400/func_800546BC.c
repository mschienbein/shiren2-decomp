#include "common.h"

typedef char *va_list;
extern unsigned char D_80161B44;
s32 func_8005ECF8(char *dst, const char *fmt, va_list args);
void func_8005457C(char *);
void func_800546BC(char *fmt, void *args) { char buf[0x100]; D_80161B44 = 0; func_8005ECF8(buf, fmt, args); func_8005457C(buf); }
