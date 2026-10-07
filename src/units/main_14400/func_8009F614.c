#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x4C]; void *vtbl; } Obj;
extern s32 D_80151E38;
void func_800D8FA8(void *object);
void func_8009F614(Obj *obj, s32 flags) {
    obj->vtbl = &D_80151E38;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
