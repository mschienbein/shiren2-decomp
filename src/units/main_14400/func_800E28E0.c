#include "common.h"

typedef struct {
    short delta;
    short index;
    s32 (*fn)(void *self, s32 arg1, s32 arg2, unsigned char arg3, s32 arg4);
} VEntry;

typedef struct {
    unsigned char pad0[0x90];
    VEntry slot18;
} VTable;

typedef struct {
    unsigned char pad0[0x24];
    VTable *vtbl;
} Unit;

s32 func_800E28E0(Unit *unit)
{
    return unit->vtbl->slot18.fn((char *)unit + unit->vtbl->slot18.delta, 0, 0x10, 0xFE, 0);
}
