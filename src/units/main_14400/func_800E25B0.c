#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x72];
    u8 flags;
} Obj800E25B0;

void func_800E25B0(Obj800E25B0 *obj) {
    obj->flags &= ~0x10;
}
