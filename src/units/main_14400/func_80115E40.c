#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* g++ 2.x vtable slot: this-adjustment delta, index, function pointer. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *self);
} VtblEntry;

typedef struct {
    u8 pad0[0x1C];
    u16 flags_1C;
    u8 pad1E[0x6];
    VtblEntry *vtbl_24;
} Obj_80115E40;

extern Obj_80115E40 *func_800B4928(void *key);

Obj_80115E40 *func_80115E40(void *self, void *key) {
    Obj_80115E40 *obj = func_800B4928(key);
    s32 reject = 0;

    if (obj != 0) {
        VtblEntry *entry = &obj->vtbl_24[2];

        if (entry->fn((u8 *)obj + entry->delta) != 0) {
            reject = 1;
        } else if (obj->flags_1C & 1) {
            reject = 1;
        }
        if (reject) {
            obj = 0;
        }
    }
    return obj;
}
