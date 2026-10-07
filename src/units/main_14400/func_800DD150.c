#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x4]; void *vtable; } Obj800DD150;
extern u8 D_80157FA8[];
void func_800D8FE8(void *ptr);
void func_800DD150(Obj800DD150 *obj, s32 flags) {
    obj->vtable = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
