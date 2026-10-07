#include "common.h"
typedef short s16;
typedef unsigned short u16;
typedef struct { s32 a; s32 b; } Tmp;
typedef struct { s16 delta; s16 index; s32 (*fn)(void *, s32, s32, unsigned char, s32); } VEntry;
typedef struct { char pad0[0x24]; VEntry *vtbl; } Obj;
s32 func_800E4454(Obj *);
s32 func_800E2074(Obj *);
void func_801F216C(u16, Obj *);
void *func_800A6538(Tmp *, Obj *, void *);
void func_800A665C(Obj *, Tmp *);
s32 func_80049CB4(s32, ...);
char *func_800A3B20(Obj *);
void func_800498E4(s32, ...);
void func_80049BF0(s32);
s32 func_800F8CAC(Obj *self, void *target) {
    s32 blocked = 0;
    s32 notReady;
    if (func_800E4454(self) != 0 || self->vtbl[18].fn((char *)self + self->vtbl[18].delta, 2, 9, blocked, 0) != 0) {
        blocked = 1;
    }
    if (blocked) {
        return 0;
    }
    notReady = func_800E2074(self) ^ 1;
    if (!notReady) {
        Tmp tmp;
        Tmp *t;
        func_801F216C(0x4BA, self);
        t = &tmp;
        func_800A6538(t, self, target);
        func_800A665C(self, t);
    } else {
        func_80049CB4(0x128, 0x1A6);
        func_80049CB4(0xAA, self);
        func_800498E4(0x1BD, func_800A3B20(self));
        func_80049BF0(0);
        func_80049CB4(0xAB, self);
    }
    return 1;
}
