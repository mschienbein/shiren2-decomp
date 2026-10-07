#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

s32 func_8004505C(s32 id, void *outKind, void *outLevel);
u16 func_800EFDAC(u8 kind, u8 variant);
char *func_80048480(u16 id);
char *func_80083C90(char *dst, char *src);
void func_800D89F0(u8 *self, char *arg) {
    u8 a;
    u8 b;

    func_8004505C(*(s32 *)(self + 0xC8), &a, &b);
    func_80083C90(arg, func_80048480(func_800EFDAC(a, b)));
}
