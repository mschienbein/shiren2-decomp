#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x4]; void *vtable_4; } Obj800DC3BC;
extern u8 D_80157FA8[];
void func_800D8FE8(Obj800DC3BC *obj);

void func_800DC3BC(Obj800DC3BC *obj, s32 flags) {
    obj->vtable_4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
