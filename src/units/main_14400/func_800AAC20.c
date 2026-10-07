#include "common.h"

extern s32 D_80142DE0[];
extern void *func_800AB16C(void *table, unsigned char pick, s32 mode);

void *func_800AAC20(s32 mode) {
    return func_800AB16C(D_80142DE0, 0, mode);
}
