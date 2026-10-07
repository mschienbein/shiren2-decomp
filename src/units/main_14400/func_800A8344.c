#include "common.h"

typedef unsigned short u16;

typedef struct {
    char pad0[0x1C];
    u16 flags;
} Obj800A8344;

void func_800A8344(Obj800A8344 *obj, u16 bits) {
    obj->flags |= bits;
}
