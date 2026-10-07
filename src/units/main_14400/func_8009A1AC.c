#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* g++ 2.x vtable slot: this-adjustment delta, index, function pointer. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *self);
} VtblEntry;

typedef struct {
    u8 pad0[0x4];
    VtblEntry *vtbl_4;
} Sub_8009A1AC;

typedef struct {
    u8 kind_0;
    u8 pad1;
    u8 flags_2;
    u8 pad3[0x5];
    VtblEntry *vtbl_8;
    Sub_8009A1AC sub_C;
} Obj_8009A1AC;

s32 func_8009A1AC(void *self, Obj_8009A1AC *obj) {
    VtblEntry *entry;
    Sub_8009A1AC *sub;

    if (obj->flags_2 & 4) {
        entry = &obj->vtbl_8[2];
        if (entry->fn((u8 *)obj + entry->delta) != 0) {
            return 1;
        }
    }
    if (obj->kind_0 == 9) {
        sub = &obj->sub_C;
        entry = &sub->vtbl_4[4];
        if (entry->fn((u8 *)sub + entry->delta) != 0) {
            return 2;
        }
    }
    return 0;
}
