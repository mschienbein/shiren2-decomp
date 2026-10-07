#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { u8 x0; u8 x1; u8 x2; } Unit;
typedef struct { s32 x0; Unit *x4; } Ref;
typedef struct { u8 pad[8]; Ref ref; } S;
extern u8 D_80142920[];
extern u32 D_8013960C;
extern void *D_801476B8;
extern void func_800A16BC(void *, u8);
extern void func_800AE518(Unit *, void *, s32, s32);
extern s32 func_80049CB4(s32, ...);
extern void func_800D0348(Ref *);
s32 func_800DD5CC(S *s){
    Ref *ref = &s->ref;
    Unit *u = ref->x4;
    func_800A16BC(D_80142920, u->x1);
    if (u->x2 & 4) {
        D_8013960C <<= 1;
        func_800AE518(u, D_801476B8, 0, 0);
        D_8013960C >>= 1;
    }
    func_80049CB4(2);
    func_800D0348(ref);
    return 1;
}
