#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xC];
    u8 flags_C;
} Obj80116AE4;

void func_80116AE4(Obj80116AE4 *obj) {
    obj->flags_C &= ~0x20;
}
