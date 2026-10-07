#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 field_0;
    s32 field_4;
} Pair;

s32 func_800A21B0(s32 a, s32 b);

void *func_800A21F0(void *out_direction, Pair *pair) {
    *(u8 *)out_direction = func_800A21B0(pair->field_4, pair->field_0) & 7;
    return out_direction;
}
