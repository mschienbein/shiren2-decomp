#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0xC]; s32 count_C; } Collection;
extern u8 D_80143094[];
extern s32 func_800AF920(void *collection, u8 index);

s32 func_800AFDBC(Collection *collection) {
    s32 i;
    s32 count = 0;
    for (i = 0; i < collection->count_C; i++) {
        s32 vacant = func_800AF920(D_80143094, i) ^ 1;
        if (vacant) {
            count++;
        }
    }
    return count;
}
