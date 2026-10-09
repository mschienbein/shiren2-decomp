#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x72];
    u8 flags_72;
} Obj800E2670;

void func_800E2670(Obj800E2670 *obj, s32 mask) {
    obj->flags_72 &= ~mask;
}
