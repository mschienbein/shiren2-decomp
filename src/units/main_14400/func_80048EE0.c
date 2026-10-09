#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Unit Unit;
struct Unit {
    u8 pad00[0x1E];
    u8 flags1E;
    u8 pad1F[0x9A - 0x1F];
    u16 flags9A;
    u8 pad9C[0xE4 - 0x9C];
    u16 flagsE4;
    u8 padE6[0x104 - 0xE6];
    Unit *field104;
};

extern Unit *func_800C5F60(void);
extern s32 func_800E1CC4(Unit *obj, s32 kind);
extern s32 func_800A6FD0(Unit *self);
extern Unit *D_801476B8;

static inline s32 unit_flagged(Unit *unit)
{
    s32 flagged = 0;

    if (unit->field104 == 0) {
        flagged = (unit->flagsE4 >> 3) & 1;
    }
    return flagged;
}

static inline s32 unit_level(Unit *unit)
{
    s32 level;

    if (func_800E1CC4(func_800C5F60(), 0)) {
        if (func_800A6FD0(unit)) {
            level = func_800E1CC4(unit, 1) ? 2 : 1;
        } else {
            level = 0;
        }
    } else if (!func_800E1CC4(unit, 1)) {
        level = 1;
    } else if (unit_flagged(D_801476B8)) {
        level = 2;
    } else {
        s32 marked = func_800A6FD0(unit) || ((unit->flags1E >> 3) & 1)
                  || (((unit->flags1E >> 4) & 1) && (unit->flags9A & 0x40));

        level = marked * 2;
    }
    return level;
}

s32 func_80048EE0(Unit *unit)
{
    return unit_level(unit) == 0;
}
