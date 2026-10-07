#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[4];
    void *unk4;
} Obj800D2A64;

typedef struct {
    u8 pad0[0x80];
    Obj800D2A64 *owner;
} Unit800D2A64;

s32 func_800A9070(s32 *it, s32 kind);
Unit800D2A64 *func_800A910C(s32 *it);
u16 func_800E08B0(Unit800D2A64 *unit);
s32 func_800A31C8(void *arg, Unit800D2A64 *unit);

s32 func_800D2A64(Obj800D2A64 *self) {
    s32 found = 0;
    s32 it = 0;

    while (func_800A9070(&it, 0x57)) {
        Unit800D2A64 *unit = func_800A910C(&it);
        s32 ok = 0;

        if (unit->owner == self && func_800E08B0(unit)) {
            ok = func_800A31C8(self->unk4, unit) != 0;
        }
        if (ok) {
            found = 1;
            break;
        }
    }
    return found;
}
