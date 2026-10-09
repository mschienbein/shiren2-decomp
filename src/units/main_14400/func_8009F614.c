#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x4C]; const void *vtbl; } Obj;
extern const unsigned char D_80151E38[144];
void func_800D8FA8(void *object);
void func_8009F614(Obj *obj, s32 flags) {
    obj->vtbl = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
