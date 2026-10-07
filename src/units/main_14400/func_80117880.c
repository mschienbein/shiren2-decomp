#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct { u8 pad0[0x90]; s16 x90; s16 pad92; s32 (*x94)(void *, s32, s32, u8, s32); } VTable;
typedef struct {
    s32 x0;
    s32 x4;
    u8 pad8[0x16];
    u8 x1E;
    u8 pad1F[5];
    VTable *vtable;
    u8 pad28[0xBC];
    u16 xE4;
} Unit;
typedef struct { s32 x0; s32 x4; } Pos;
typedef struct { s32 x0; s32 x4; } Iter;
extern u32 D_8013960C;
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 id, ...);
s32 func_800E1CC4(Unit *unit, s32 mode);
s32 func_800A8FC8(Iter *it, s32 kind);
Unit *func_800A910C(Iter *it);
/* Actor-effect slot +0x44 supplies self and actor; this override ignores self. */
void func_80117880(void *obj, Unit *unit) {
    Pos pos;
    Pos *posPtr;
    Iter it;
    s32 notSet = ((unit->x1E >> 2) & 1) ^ 1;
    if (notSet) {
        return;
    }
    posPtr = &pos;
    pos.x0 = unit->x0;
    posPtr->x4 = unit->x4;
    func_80049CB4(0x10A, &pos);
    notSet = ((unit->xE4 >> 3) & 1) ^ 1;
    if (notSet) {
        unit->xE4 |= 8;
        if (func_800E1CC4(unit, 0)) {
            D_8013960C <<= 1;
            unit->vtable->x94((u8 *)unit + unit->vtable->x90, 1, 0, 0, 0);
            D_8013960C >>= 1;
        }
        it.x0 = 0;
        while (func_800A8FC8(&it, 0x7C)) {
            Unit *other = func_800A910C(&it);
            if (func_800E1CC4(other, 1)) {
                func_80049CB4(0x1F, other);
            }
        }
        func_800498E4(0x88);
        func_80049CB4(0xDB);
    }
}
