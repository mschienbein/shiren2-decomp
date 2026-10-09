#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pos801058A8;
typedef struct { u8 value; } Dir801058A8;

typedef struct Unit801058A8 {
    Pos801058A8 pos_0;
    u8 dir_8;
    u8 pad9[0x4F];
    struct Unit801058A8 *target_58;
    u8 pad5C[0x8];
    Pos801058A8 home_64;
    u8 pad6C[0x6];
    u8 flags_72;
    u8 pad73[0x17];
    u8 level_8A;
} Unit801058A8;

u32 func_800B1C6C(Pos801058A8 *pos);
u8 func_800A6420(Unit801058A8 *unit, Unit801058A8 *target);
u16 func_800E08B0(Unit801058A8 *unit);
u16 func_800E08F0(Unit801058A8 *unit);
void *func_800A6538(Dir801058A8 *out, Unit801058A8 *unit, Pos801058A8 *pos);
void func_800A665C(Unit801058A8 *unit, u8 *dir);
s32 func_800A23E8(Pos801058A8 *from, Pos801058A8 *to);
void *func_800A2594(Pos801058A8 *out, Pos801058A8 *from, Dir801058A8 dir);
s32 func_800A4CC4(Unit801058A8 *unit, Pos801058A8 *pos, Dir801058A8 *dir);
void func_800A4F58(Unit801058A8 *unit, Dir801058A8 *dir);
s32 func_800F1024(Unit801058A8 *unit);
void func_800F06E4(Unit801058A8 *unit);
s32 func_800E7104(Unit801058A8 *unit);
void *func_800A6CC0(Pos801058A8 *out, Unit801058A8 *unit);
s32 func_800A50AC(Unit801058A8 *unit);
s32 func_800E7EB4(Unit801058A8 *unit);
s32 func_800E7794(Unit801058A8 *unit);

/* Unit vtable slot +0xA4 (D_80159440 family): s32 (*)(void *self). */
s32 func_801058A8(void *self) {
    Unit801058A8 *unit = self;
    Pos801058A8 here;
    Pos801058A8 dest;
    Pos801058A8 step;
    Dir801058A8 face;
    Dir801058A8 turn;
    Pos801058A8 *herePtr = &here;
    Unit801058A8 *foe;
    Pos801058A8 *target;
    Pos801058A8 *home;
    s32 inRoom;
    s32 weak;
    s32 ok;
    s32 moved;

    unit->flags_72 &= ~2;
    herePtr->x = unit->pos_0.x;
    herePtr->y = unit->pos_0.y;
    inRoom = 0;
    if (func_800B1C6C(herePtr) & 0x2000) {
        inRoom = 1;
    }
    foe = unit->target_58;
    target = &foe->pos_0;
    switch (func_800A6420(unit, foe)) {
    case 0:
        weak = func_800E08B0(unit) <= unit->level_8A;
        if (weak) {
            goto retreat;
        }
        func_800F06E4(unit);
        return 0;
    case 1:
    case 2:
        dest.x = target->x;
        dest.y = target->y;
        if (inRoom) {
            func_800A6538(&face, unit, &dest);
            func_800A665C(unit, &face.value);
            step.x = dest.x;
            step.y = dest.y;
            if (func_800A23E8(herePtr, &step) == 1) {
                weak = func_800E08B0(unit) <= unit->level_8A;
                moved = 0;
                if (weak) {
                    turn.value = (unit->dir_8 + 4) & 7;
                    func_800A2594(&step, herePtr, turn);
                    if (func_800B1C6C(&step) & 0x2000) {
                        moved = func_800A4CC4(unit, herePtr, &turn) != 0;
                    }
                    if (moved) {
                        func_800A4F58(unit, &turn);
                        return 1;
                    }
                }
                ok = 0;
                if (!(func_800B1C6C(&dest) & 0x4000)) {
                    ok = func_800F1024(unit) != 0;
                }
                if (ok) {
                    func_800F06E4(unit);
                }
                return 0;
            }
            weak = func_800E08B0(unit) <= unit->level_8A;
            if (weak) {
                return 0;
            }
        } else {
            weak = func_800E08B0(unit) <= unit->level_8A;
            if (weak) {
                goto retreat;
            }
        }
        break;
    default:
        if (inRoom) {
            ok = func_800E08B0(unit) == func_800E08F0(unit);
            if (!ok) {
                return 0;
            }
            func_800A6CC0(&dest, unit);
            if (func_800B1C6C(&dest) & 0x2000) {
                return func_800A50AC(unit);
            }
        }
        if (func_800E08B0(unit) <= unit->level_8A) {
            goto retreat;
        }
        break;
    }
    return func_800E7104(unit);

retreat:
    home = &unit->home_64;
    if (!(func_800B1C6C(home) & 0x2000)) {
        unit->home_64.x = 0;
        home->y = 0;
    }
    if (func_800E7EB4(unit) != 0) {
        return 1;
    }
    return func_800E7794(unit);
}
