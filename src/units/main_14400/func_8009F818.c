#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
extern char D_8015306C[];
s32 func_8004505C(s32, u8 *, u8 *);
u16 func_800EFDAC(u8, u8);
char *func_80048480(u16 id);
char *func_80083C90(char *dst, char *src);
s32 func_800D81D4(s32);
s32 func_8005EF08(char *dst, const char *fmt, ...);
char *func_80083D04(char *dst, char *src);
void func_8009F818(s32 unused, s32 id, char *out) {
    char buf[16];
    u8 a, b;
    s32 n;
    if (func_8004505C(id, &a, &b)) {
        func_80083C90(out, func_80048480(func_800EFDAC(a, b)));
        n = func_800D81D4(id);
        if (n > 0) {
            func_8005EF08(buf, D_8015306C, n);
            func_80083D04(out, buf);
        }
    } else {
        *out = 0;
    }
}
