#include "common.h"

typedef unsigned char u8;

typedef struct Position {
    s32 x;
    s32 y;
} Position;

typedef struct Dir801070A8 {
    u8 value;
} Dir801070A8;

typedef struct Object {
    Position position;
    u8 pad_08[0x58 - 0x8];
    struct Object *target_58;
    u8 pad_5C[0x89 - 0x5C];
    u8 range_89;
    u8 pad_8A[0xA0 - 0x8A];
    s32 mode_A0;
    Position home_A4;
} Object;

s32 func_800A251C(Position *x, Object *y);
extern s32 func_800E1CD4(Object *, s32);
s32 func_800E65C0(Object *unit, Position *target, s32 mode);
s32 func_800E8350(Object *);
u8 func_800A6420(Object *obj, Object *target);
void *func_800A65E4(Dir801070A8 *p, Object *q, void *target);
void func_800A665C(Object *obj, u8 *value);
s32 func_800E0F40(Object *obj);
extern s32 func_800A65B8(Object *, void *);
u32 func_800B1C6C(Object *pos);
s32 func_800A6E90(void *p);
extern s32 func_800F1024(Object *obj);
void func_800F06E4(Object *obj);
s32 func_800E7104(Object *unit);

/* Life vtable slot (pointer at 0x8015C2AC): AI step for a unit that guards / returns home. */
s32 func_801070A8(Object *self) {
    Object *target = self->target_58;
    Object *prey = target;
    s32 mode = self->mode_A0;
    s32 idle = mode == 1;
    s32 returning;

    if (idle) {
        self->home_A4 = self->position;
        return 0;
    }
    returning = mode == 3;
    if (returning) {
        s32 away = func_800A251C(&self->home_A4, self) != 1;

        if (away) {
            if (func_800E1CD4(self, 0x10) != 0) {
                return func_800E8350(self);
            }
            return func_800E65C0(self, &self->home_A4, 0);
        }
        if (func_800A6420(self, target) != 0) {
            Dir801070A8 dir;

            func_800A65E4(&dir, self, target);
            func_800A665C(self, &dir.value);
            return 0;
        }
        func_800F06E4(self);
        return 0;
    }
    {
        u8 sight = func_800E0F40(self);

        if (sight == 3) {
            s32 attack = 0;

            if (func_800A6420(self, prey) != sight && func_800A65B8(self, prey) <= self->range_89
                && !(func_800B1C6C(prey) & 0x4000)) {
                attack = func_800A6E90(prey) == 0;
            }
            if (attack && func_800F1024(self) != 0) {
                func_800F06E4(self);
                return 0;
            }
            if (self->mode_A0 != 0) {
                return 0;
            }
        }
    }
    if (func_800E1CD4(self, 0x10) != 0) {
        return func_800E8350(self);
    }
    return func_800E7104(self);
}
