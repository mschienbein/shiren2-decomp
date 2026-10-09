#include "common.h"

typedef unsigned char u8;

typedef struct {
    char text[4];
} Name800B0A70;

extern u8 D_8014313C[];
extern const u8 D_8015488C[8];
extern Name800B0A70 D_80143144[];
extern char D_801C91C0[];

char *func_800B0A70(u8 id, s32 force) {
    s32 outOfRange = (id >= 1 && id <= 40) == 0;

    if (outOfRange) {
        return 0;
    }
    if (force != 0 || (D_8014313C[(id - 1) >> 3] & D_8015488C[(id - 1) & 7])) {
        *(Name800B0A70 *)D_801C91C0 = D_80143144[id - 1];
        D_801C91C0[4] = 0;
        return D_801C91C0;
    }
    return 0;
}
