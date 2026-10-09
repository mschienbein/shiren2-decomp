#include "common.h"

typedef struct { s32 word[4]; } PoolRecord16;
extern s32 D_8013D454;
extern s32 D_8013D450;
extern PoolRecord16 *D_801A7150[2];
extern PoolRecord16 *D_801A7158;

void *func_800705F4(s32 count) {
    PoolRecord16 *current = D_801A7158;
    if (D_8013D454 == 0) {
        return 0;
    }
    if ((u32)(current - D_801A7150[D_8013D450] + count) > 1500) {
        return 0;
    }
    D_801A7158 = current + count;
    return current;
}
