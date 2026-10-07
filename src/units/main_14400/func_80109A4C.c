#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 v; } Byte;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
typedef struct { s32 x; s32 y; } Pt;
typedef struct { s32 unk[10]; } Iter;
typedef struct Obj {
    u8 pad0[8];
    u8 unk8;
    u8 pad9[0x54 - 9];
    u8 unk54;
    u8 pad55[3];
    struct Obj *unk58;
    u8 pad5C[0xC0 - 0x5C];
    s32 unkC0;
} Obj;
u8 func_800A6420(Obj *, Obj *);
s32 func_800EE504(Obj *);
void func_800A2F80(Byte *, s32);
void *func_800A7DEC(void *obj);
s32 func_800A46BC(void *obj, void *pos, void *dir);
void *func_800A39C0(void *out, void *pos, void *dir, s32 dist);
void *func_800B4928(Pt *pos);
void *func_800C5150(void *self, void *position, ShirenDirection direction, s32 limit, u32 mask);
void *func_800C51B8(void *iterator);
s32 func_800A44F4(void *self, void *target);
s32 func_800A674C(Obj *, Obj *);
s32 func_800A650C(Obj *, Obj *);
void func_800A665C(Obj *, Byte *);
Obj *func_800A492C(Obj *, s32, s32, s32);
s32 func_800E7104(Obj *);
s32 func_800E776C(void *obj, u8 mode);
s32 func_80109A4C(Obj *self, u8 mode) {
    Pt pos;
    Iter it;
    Byte dir;
    Byte bestDir;
    ShirenDirection facing;
    Obj *target = self->unk58;
    Obj *found;
    Obj *best;
    s32 bestDist;
    s32 dist;
    s32 count;
    s32 ok;
    s32 valid;
    u8 rel;
    u8 kind;

    self->unkC0 = 0;
    rel = func_800A6420(self, target);
    if (rel == 0) {
        self->unk54 |= 4;
        return 0;
    }
    best = 0;
    bestDist = 0xFF;
    dir.v = (self->unk8 - 1) & 7;
    count = (func_800EE504(self) != 0) * 8;
    while (1) {
        count--;
        if (count == -1) {
            break;
        }
        func_800A2F80(&dir, 1);
        ok = func_800A46BC(self, func_800A7DEC(self), &dir) == 1;
        if (!ok) {
            continue;
        }
        func_800A39C0(&pos, self, &dir, 1);
        func_800B4928(&pos);
        func_800A39C0(&pos, self, &dir, 1);
        facing.value = dir.v;
        func_800C5150(&it, &pos, facing, 5, 0x4000);
        found = func_800C51B8(&it);
        if (found == 0) {
            continue;
        }
        valid = 0;
        if (func_800A44F4(self, found) == 2) {
            valid = func_800A674C(self, found) != 0;
        }
        if (!valid) {
            continue;
        }
        kind = func_800A6420(self, found);
        if (kind == 0) {
            self->unk58 = found;
            self->unk54 |= 4;
            return 0;
        }
        if (kind == 3) {
            continue;
        }
        dist = func_800A650C(self, found);
        if (dist < bestDist) {
            best = found;
            bestDist = dist;
            bestDir = dir;
        }
    }
    if (best != 0) {
        func_800A665C(self, &bestDir);
        self->unk58 = best;
        self->unkC0 = 1;
        self->unk54 |= 4;
        return 0;
    }
    if (mode != 1) {
        if (rel == 3) {
            target = func_800A492C(self, 2, 1, 1);
        }
        if (target != 0) {
            self->unk58 = target;
            return func_800E7104(self);
        }
    }
    return func_800E776C(self, 3);
}
