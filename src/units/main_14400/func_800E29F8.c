#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* g++ 2.x vtable slot; slot 18 targets (func_800F212C, func_800F426C) take
 * (self, s32, s32, u8, s32) and return s32. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *self, s32 a, s32 b, u8 c, s32 d);
} VtblEntry;

typedef struct {
    u8 pad0[0x24];
    VtblEntry *vtbl24;
} Obj_800E29F8;

s32 func_800E29F8(Obj_800E29F8 *obj) {
    VtblEntry *entry = &obj->vtbl24[18];

    return entry->fn((u8 *)obj + entry->delta, 1, 0xE, 0, 0);
}
