#include "common.h"

typedef unsigned char u8;

/* One 0xE4-byte unit record. */
typedef struct {
    u8 data[0xE4];
} Unit;
typedef struct { Unit base; u8 player_fields[0x28]; } Player;

/* 0x20-byte unit iterator: cursor at +0, Pair pointer at +4 (func_800A9204),
 * position pair at +8 and rectangle at +0x10. */
typedef struct {
    s32 cur;
    void *position_04;
    s32 position_08[2];
    s32 rect_10[4];
} UnitIter;

extern Player D_801C35E0;  /* complete 0x10C-byte player */
extern Unit D_801C36EC[];  /* thirty records; first 29 are normal iterator slots */

Unit *func_800A942C(UnitIter *it)
{
    s32 index = it->cur;

    if (index == 0x1D) {
        it->cur = 0x1E;
        return &D_801C35E0.base;
    }
    it->cur = index + 1;
    return &D_801C36EC[index];
}
