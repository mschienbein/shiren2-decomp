#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 value; } Dir;
typedef struct { s32 a, b; } R;
u32 func_80048AC8(void);
void func_80046B3C(void *, Dir);
void *func_800A2594(void *, void *, Dir);
void func_800A592C(void *, R *);
void func_800A4F58(void *a, Dir *b) {
    R r;
    if (func_80048AC8()) func_80046B3C(a, *b);
    func_800A2594(&r, a, *b);
    func_800A592C(a, &r);
}
