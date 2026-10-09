#include "common.h"

typedef unsigned char u8;
typedef struct UnitVTable {
    u8 pad_00[0x90];
    short this_delta_90;
    short slot_92;
    s32 (*check_94)(void *, s32, s32, u8, s32);
} UnitVTable;
typedef struct Unit {
    u8 pad_00[0x24];
    UnitVTable *vtable_24;
} Unit;

s32 func_800E30BC(Unit *unit)
{
    return unit->vtable_24->check_94((u8 *)unit + unit->vtable_24->this_delta_90,
                                   0, 3, 0xFE, 0);
}
