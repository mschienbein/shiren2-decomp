#include "common.h"

typedef struct { s32 x0, x4; } P;
void func_800A59A4(P *);
s32 func_800A4360(P *, P *);
void func_800A5A70(P *, u32);
s32 func_800B48C0(P *, P *);
void func_800A5A04(P *);
s32 func_80049CB4(s32, ...);
void func_800A592C(P *a, P *b) {
    func_800A59A4(a);
    *a = *b;
    func_800A5A70(a, func_800A4360(a, b));
    func_800B48C0(b, a);
    func_800A5A04(a);
    func_80049CB4(0xD8, b);
}
