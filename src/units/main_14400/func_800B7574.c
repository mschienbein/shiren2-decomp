#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* g++-style vtable: 8-byte entries { s16 delta; s16 index; void (*fn)(void *, s32); } */
typedef struct {
    s16 delta;
    s16 index;
    void (*fn)(void *self, s32 flags);
} VtEntry800B7574;

typedef struct {
    s32 field_0;
    s32 active;
} Sub800B7574;

typedef struct {
    u8 pad0[0xA];
    u8 kind;
    u8 padB[0x24 - 0xB];
    VtEntry800B7574 *vtable;
    u8 pad28[0x84 - 0x28];
    Sub800B7574 sub;
} Obj800B7574;

extern u8 D_80147490;

s32 func_800B7574(Obj800B7574 *obj) {
    Sub800B7574 *sub = obj != 0 ? &obj->sub : 0;

    if (sub->active) {
        D_80147490 |= 1 << (obj->kind - 0x18);
    }
    if (obj != 0) {
        VtEntry800B7574 *entry = &obj->vtable[1];
        entry->fn((char *)obj + entry->delta, 3);
    }
    return 0;
}
