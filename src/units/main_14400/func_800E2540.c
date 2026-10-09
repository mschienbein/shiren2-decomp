#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pair800E2540;

typedef struct {
    char pad00[0x5C];
    Pair800E2540 pair5C;
} Obj800E2540;

void func_800E2540(Obj800E2540 *obj, Pair800E2540 *src) {
    obj->pair5C = *src;
}
