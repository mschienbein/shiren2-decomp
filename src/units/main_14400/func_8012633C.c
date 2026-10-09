#include "common.h"
typedef unsigned char u8;
extern unsigned char D_80143094[];
extern u8 D_801CA650[];
extern s32 func_800AFD08(void *table, void *obj);
/* self: owner receiver supplied by func_80126480's call; unused here. */
s32 func_8012633C(void *self, void *item) {
    u32 index = (u8)func_800AFD08(D_80143094, item);
    s32 result;
    if (index == 0xFF)
        result = 1;
    else
        result = (D_801CA650[index >> 3] >> (index & 7)) & 1;
    return result;
}
