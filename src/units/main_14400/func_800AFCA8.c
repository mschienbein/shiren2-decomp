#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
extern const u8 D_80154894[8];
void func_800AFCA8(ItemPool *pool, u8 index) { s32 bit = index; pool->occupancy[bit >> 3] &= D_80154894[bit & 7]; }
