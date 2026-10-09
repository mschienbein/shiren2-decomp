#include "common.h"

typedef unsigned char u8;
extern u8 D_80148644[];
extern const u8 D_8015488C[8];
u8 func_800AC1AC(u8 id);
s32 func_80112D60(u8 id) {
    s32 bit;
    if (func_800AC1AC(id) != 2) return 0;
    bit = id - 0x17;
    return (D_80148644[bit >> 3] & D_8015488C[bit & 7]) != 0;
}
