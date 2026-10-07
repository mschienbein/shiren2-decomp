#include "common.h"

typedef struct { unsigned char pad[8]; unsigned char f8; } Item;
s32 func_800CC6E4(Item *it);
s32 func_800CC6F0(Item *it) {
    if (func_800CC6E4(it)) {
        return 0;
    }
    return (u32)(it->f8 - 0x2D) < 12;
}
