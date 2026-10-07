#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 unk0;
    s32 unk4;
} Pair8009B178;

extern u8 D_801528BC[];
extern u8 D_801528C0[];

Pair8009B178 func_8009B178(void *self, Pair8009B178 *src) {
    Pair8009B178 result;

    result.unk0 = src->unk0;
    if (result.unk0 == 0) {
        result.unk4 = D_801528BC[D_801528C0[src->unk4]] - 1;
    } else {
        result.unk4 = src->unk4 * 5;
    }
    return result;
}
