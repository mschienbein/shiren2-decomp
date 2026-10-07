#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

char *func_80048480(u16 id);
u16 func_800AE710(void *obj);
char *func_800AC990(void *obj);
s32 func_8005EF08(char *dst, const char *fmt, ...);
void *func_80116C6C(void *obj, void *dst) {
    char *a = func_80048480(0x240);
    s32 b = func_800AE710(obj) & 0xFFFF;

    func_8005EF08(dst, a, b, func_800AC990(obj));
    return dst;
}
