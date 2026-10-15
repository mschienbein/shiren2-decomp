#include "map_unit_iterator.h"

typedef unsigned char u8;

/* One 0xE4-byte unit record. */
typedef struct {
    u8 data[0xE4];
} Unit;
typedef struct { Unit base; u8 player_fields[0x28]; } Player;


extern Player D_801C35E0;  /* complete 0x10C-byte player */
extern Unit D_801C36EC[];  /* thirty records; first 29 are normal iterator slots */

void *func_800A942C(MapUnitIterator *it)
{
    s32 index = it->cursor;

    if (index == 0x1D) {
        it->cursor = 0x1E;
        return &D_801C35E0.base;
    }
    it->cursor = index + 1;
    return &D_801C36EC[index];
}
