#include "common.h"

/* D_801476B8 unit; a non-NULL +0x104 unit takes its place. */
typedef struct Unit {
    unsigned char pad0[0x104];
    struct Unit *field_104;
} Unit;

extern Unit *D_801476B8;

Unit *func_800C5F60(void)
{
    Unit *player = D_801476B8;
    Unit *unit = player->field_104;

    if (unit == 0) {
        unit = player;
    }
    return unit;
}
