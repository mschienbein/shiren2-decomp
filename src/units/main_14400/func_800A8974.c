#include "common.h"

typedef unsigned char u8;

extern u8 D_801C35E0[]; /* the player object (id 0); only its address is compared */
extern u8 D_801C51A4[];
extern u8 D_8015488C[];
u8 func_800A8C00(void *obj);
s32 func_800A8974(void *obj) {
    u8 id;
    s32 result;
    if (obj == D_801C35E0) {
        return 0;
    }
    id = func_800A8C00(obj);
    result = 0;
    if (id == 0xFF || !(D_801C51A4[(id - 1) >> 3] & D_8015488C[(id - 1) & 7])) {
        result = 1;
    }
    return result;
}
