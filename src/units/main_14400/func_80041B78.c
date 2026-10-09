#include "common.h"

typedef struct {
    short delta;
    short index;
    u32 (*fn)(void *self);
} VEntry;

typedef struct {
    unsigned char pad0[0x70];
    VEntry getCur;
} VTable;

typedef struct {
    unsigned char pad0[0x24];
    VTable *vtbl;
} Unit;

extern Unit *D_801476B8;

s32 func_80041B78(void)
{
    Unit *player = D_801476B8;

    return (unsigned short)player->vtbl->getCur.fn((char *)player + player->vtbl->getCur.delta);
}
