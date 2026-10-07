#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0x10]; s16 offset; u8 pad12[2]; s32 (*func)(void *self); } VEntry;
typedef struct { s32 field_0; VEntry *vtable; } Inner;
typedef struct { u8 pad0[0x8C]; Inner *inner; } Obj;
void func_800F39B4(Obj *obj) {
    Inner *inner = obj->inner;
    VEntry *entry = inner->vtable;

    entry->func((u8 *)inner + entry->offset);
}
