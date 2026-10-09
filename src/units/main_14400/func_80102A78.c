#include "common.h"

typedef unsigned short u16;

typedef struct {
    u32 high : 8;
    u32 flag23 : 1;
    u32 low : 23;
} Flags20;

typedef struct {
    char pad00[0x20];
    Flags20 flags20;
    char pad24[0xE4 - 0x24];
    u16 flagsE4;
} Unit;

typedef struct {
    char pad00[0x9A];
    u16 flags9A;
} Obj80102A78;

extern Unit *D_801476B8;

static inline s32 flag23_set(Flags20 *flags) {
    return flags->flag23;
}

s32 func_80102A78(Obj80102A78 *self) {
    s32 result = 0;

    if (self->flags9A & 0x40) {
        Unit *unit = D_801476B8;
        Flags20 flags = unit->flags20;

        if (flag23_set(&flags) || (unit->flagsE4 >> 3) & 1) {
            result = 1;
        }
    }
    return result;
}
