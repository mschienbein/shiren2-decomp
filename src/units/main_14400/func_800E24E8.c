#include "common.h"

/* Position pair at +0x64 (also read by func_800E24F0 and written by func_800E2504). */
typedef struct {
    s32 x;
    s32 y;
} Pair;

/* Partial view: only the pair's address is taken. */
typedef struct {
    unsigned char pad_00[0x64];
    Pair pair_64;
} Obj;

Pair *func_800E24E8(Obj *obj) {
    return &obj->pair_64;
}
