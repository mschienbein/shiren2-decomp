#include "common.h"
#include "item_records.h"
extern const unsigned char D_8015488C[8];
void func_800AFCD8(ItemPool *pool, unsigned char index) { s32 bit = index; pool->occupancy[bit >> 3] |= D_8015488C[bit & 7]; }
