#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pair;

extern s32 func_800ADBC8(void *a, void *b, Pair *pair, s32 d, s32 e);

s32 func_800ADB94(void *a, void *b, Pair *src, s32 d) {
    Pair pair;
    Pair *dst = &pair;

    dst->x = src->x;
    dst->y = src->y;
    return func_800ADBC8(a, b, dst, d, 1);
}
