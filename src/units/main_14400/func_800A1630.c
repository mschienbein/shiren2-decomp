#include "common.h"

typedef unsigned char u8;

/* Two packed bit sets: ids 50..88 in the first five bytes, ids 89..117 after them. */
typedef struct {
    u8 low[5];
    u8 high[4];
} BitSets;

extern const u8 D_8015488C[8];

s32 func_800A1630(BitSets *sets, u8 id)
{
    s32 bit;

    if (id >= 50 && id < 89) {
        bit = id;
        bit -= 50;
        return (sets->low[bit >> 3] & D_8015488C[bit & 7]) != 0;
    }
    if (id >= 89 && id < 118) {
        bit = id;
        bit -= 89;
        return (sets->high[bit >> 3] & D_8015488C[bit & 7]) != 0;
    }
    return 0;
}
