#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x2]; u8 flags; u8 pad3[0x5]; void *vtable; u8 padC[0x4]; s32 unk10; } Obj80129080;
extern u8 D_801607F8[];
void *func_80117230(void *obj, s32 kind);
Obj80129080 *func_80129080(Obj80129080 *obj) {
    func_80117230(obj, 0xF4);
    obj->vtable = D_801607F8;
    obj->unk10 = 1;
    obj->flags |= 0x10;
    return obj;
}
