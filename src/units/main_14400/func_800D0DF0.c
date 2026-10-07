#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

/* Partial view of slot +0x64: lookup(self, key, flags), returning a boolean. */
typedef struct {
    u8 pad0[0x60];
    s16 this_offset;
    u8 pad62[2];
    s32 (*method)(void *self, void *key, s32 flags);
} VTable800D0DF0;
typedef struct { s32 field_0; VTable800D0DF0 *vtable; } Obj800D0DF0;
s32 func_800D0DF0(Obj800D0DF0 *obj, void *key, void *unused, s32 flags) {
    VTable800D0DF0 *vt = obj->vtable;
    return vt->method((u8 *)obj + vt->this_offset, key, flags);
}
