#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s16 delta; s16 pad; void *func; } VEntry;
typedef struct { s32 field_0; VEntry *vtable; } Obj;
void func_800CD304(Obj *obj, u32 value) {
    if (value < ((s32 (*)(void *))obj->vtable[4].func)((u8 *)obj + obj->vtable[4].delta)) {
        ((void (*)(void *, s32))obj->vtable[9].func)((u8 *)obj + obj->vtable[9].delta, value);
    }
}
