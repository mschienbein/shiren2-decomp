#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos800AC6F8;
typedef struct { u8 kind; u8 subkind; s8 flags; u8 pad3[2]; s8 owner; } Item800AC6F8;

typedef struct {
    u8 pad0[0x1E];
    u8 flags1E;
} Unit800AD714;

Unit800AD714 *func_800B4928(Pos800AC6F8 *pos);
s32 func_800AD6AC(Item800AC6F8 *item, Pos800AC6F8 *pos);
s32 func_800B4F74(Pos800AC6F8 *pos);

/* Can `item` be placed at `dest`? */
s32 func_800AD714(Item800AC6F8 *item, Pos800AC6F8 *dest)
{
    Unit800AD714 *unit = func_800B4928(dest);
    s32 ok;

    if (unit != 0) {
        if (item->kind == 10) {
            return 0;
        }
        if ((unit->flags1E >> 1) & 1) {
            return 0;
        }
    }
    ok = 0;
    if (func_800AD6AC(item, dest)) {
        ok = func_800B4F74(dest) == 0;
    }
    return ok;
}
