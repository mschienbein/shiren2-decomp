#include "common.h"

typedef unsigned short u16;
s32 D_801398BC = 0;
extern void func_80053AE8(s32);
extern void func_8005493C(void);
extern char *func_80048480(u16);
extern char *func_8004247C(void);
extern s32 func_80041AF4(void);
extern s32 func_80041B1C(void);
extern s32 func_80041A74(void);
extern s32 func_80041B44(void);
extern s32 func_80041B78(void);
extern s32 func_80041AA0(void);
extern s32 func_80041BAC(void);
extern void func_80053B44(s32, s32, s32, s32, s32, const char *, ...);

void func_800549AC(s32 enabled) {
    char *a, *b;
    s32 c, d, e, f, g, h;
    if (enabled == 0) {
        if (D_801398BC) {
            func_80053AE8(0);
            D_801398BC = 0;
        }
    } else {
        func_8005493C();
        a = func_80048480(0x4D9);
        b = func_8004247C();
        c = func_80041AF4();
        d = func_80041B1C();
        e = func_80041A74();
        f = func_80041B44();
        g = func_80041B78();
        h = func_80041AA0();
        func_80053B44(0, 5, 0x15, 0x1E, 6, a, b, c, d, e, f, g, h, func_80041BAC());
        D_801398BC = 1;
    }
}
