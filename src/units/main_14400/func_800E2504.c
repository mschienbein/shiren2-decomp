#include "common.h"

typedef struct Pair800E2504 {
    s32 x;
    s32 y;
} Pair800E2504;

/* Partial view: only the pair at 0x64 is written. */
typedef struct Obj800E2504 {
    unsigned char pad_00[0x64];
    Pair800E2504 pair_64;
} Obj800E2504;

void func_800E2504(Obj800E2504 *obj, Pair800E2504 *pair)
{
    obj->pair_64 = *pair;
}
