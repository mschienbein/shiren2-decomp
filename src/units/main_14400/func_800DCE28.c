#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern const s32 D_80157FA8[];
typedef struct { u8 pad0[0x4]; const s32 *vtable; } Obj800DCE28;
void func_800D8FE8(Obj800DCE28 *obj);
void func_800DCE28(Obj800DCE28 *obj, s32 flags) {
    obj->vtable = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
