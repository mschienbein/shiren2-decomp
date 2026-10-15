#include "common.h"
#include "item_records.h"

extern const unsigned char D_8015488C[8];
s32 func_800AF920(ItemPool *pool, unsigned char n) { s32 i = n; return (pool->occupancy[i >> 3] & D_8015488C[i & 7]) != 0; }
