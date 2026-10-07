#include "common.h"

typedef unsigned char u8;

/* Seven 16-byte records (contents unknown); the table below is owned by this unit. */
extern u8 D_80143094[];
extern u8 D_801430A4[];
extern u8 D_801430B4[];
extern u8 D_801430C4[];
extern u8 D_801430D4[];
extern u8 D_801430E4[];
extern u8 D_801430F4[];

void *const D_80153AE4[7] = {
    D_80143094, D_801430A4, D_801430B4, D_801430C4,
    D_801430D4, D_801430E4, D_801430F4,
};

void *func_800AF818(u32 index)
{
    if (index >= 7) {
        return 0;
    }
    return D_80153AE4[index];
}
