#include "common.h"

typedef unsigned char u8;

extern u8 D_80147620[];

u8 func_800C57A0(void *rng);
s32 func_800B89D8(void *map, u8 x, u8 y, u8 dir);

/* Place at (x, y) facing one of two directions picked by a coin flip. */
void func_800BA188(void *map, u8 x, u8 y, s32 dir_a, s32 dir_b) {
    u8 dir = (func_800C57A0(D_80147620) & 1) ? dir_a : dir_b;

    func_800B89D8(map, x, y, dir);
}
