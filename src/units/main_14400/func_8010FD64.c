#include "common.h"

typedef unsigned char u8;
typedef struct {
    u32 pad:7;
    u32 locked:1;
    u32 rest:24;
} Flags;
typedef struct {
    u8 pad0[9];
    u8 unk9;
    u8 padA[0x14];
    u8 unk1E;
    u8 pad1F;
    Flags flags;
} Unit;
typedef struct {
    Unit *unit;
} Ctx;
typedef struct {
    s32 x;
    s32 y;
} Pos;
Unit *func_800B4928(Pos *);
s32 func_800A44F4(Unit *, Unit *);
s32 func_800E1D14(Unit *, s32);
s32 func_800E1CC4(Unit *, s32);
s32 func_800A58B8(Unit *);
s32 func_800B58E4(s32, s32, s32, s32);
u32 func_800B1C6C(Pos *);
s32 func_8010BEC4(void *, u8);

static inline s32 is_locked(Flags *copy, Unit *unit) {
    *copy = unit->flags;
    return copy->locked;
}

Unit *func_8010FD64(void *self, Ctx *ctx, Pos *pos, s32 checkStatus)
{
    Unit *target = func_800B4928(pos);
    s32 relation;
    s32 bad;
    s32 ok;
    s32 myType;
    s32 myLevel;
    s32 theirType;

    if (target == 0) {
        return 0;
    }
    relation = func_800A44F4(ctx->unit, target);
    if ((target->unk1E >> 1) & 1) {
        ok = 1;
    } else {
        if (checkStatus) {
            checkStatus = func_800E1D14(ctx->unit, 0x13) || func_800E1CC4(ctx->unit, 4);
        }
        if (checkStatus) {
            ok = 1;
        } else {
            ok = relation == 2;
        }
    }
    if (!ok) {
        return 0;
    }
    myType = ctx->unit->unk9 & 0xF;
    myLevel = func_800A58B8(ctx->unit);
    theirType = target->unk9 & 0xF;
    if ((func_800B58E4(myType, myLevel, theirType, func_800A58B8(target)) ^ 1) != 0) {
        bad = !(func_800B1C6C(pos) & 0x2000) || (target->unk9 & 0xF) != 1 || !(u8)func_8010BEC4(self, 0x4E);
        if (bad) {
            return 0;
        }
    }
    bad = 0;
    if (func_800B1C6C(pos) & 0x4000) {
        Flags flags;

        if (!is_locked(&flags, ctx->unit)) {
            bad = !(u8)func_8010BEC4(self, 0x78);
        }
    }
    if (bad) {
        return 0;
    }
    return target;
}
