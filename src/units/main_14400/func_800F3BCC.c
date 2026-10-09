#include "common.h"

typedef unsigned short u16;

typedef struct {
    char pad0[0x9A];
    u16 flags;
} Obj800F3BCC;

void func_800F3BCC(Obj800F3BCC *obj, s32 mask) {
    obj->flags &= ~mask;
}
