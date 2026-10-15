#include "func_801EA2D8_13C1F8/types.h"

extern "C" List *func_801EA2D8_13C1F8(s32 id) {
    OvlBInventoryUnit *unit = func_800A8CB0((u8)func_801E8D0C_13AC2C((u16)id, 0));
    /* Canonical func_800A8974 returns s32 0 or 1; invert that binary result
     * (same form as the matched sibling caller func_801EA91C_13C83C). */
    s32 visible = func_800A8974(unit) ^ 1;
    if (visible) {
        if (unit->flags_1E & 0xC) {
            return unit->table_24->inventory_9C((u8 *)unit + unit->table_24->delta_98);
        }
        if (id == 0x5A) {
            return &unit->list_8C;
        }
    }
    return 0;
}
