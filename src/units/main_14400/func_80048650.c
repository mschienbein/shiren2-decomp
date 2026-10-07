#include "common.h"

typedef short s16;

/* g++-style vtable: 8-byte entries { s16 delta; s16 index; void (*fn)(void *); } */
typedef struct {
    s16 delta;
    s16 index;
    void (*fn)(void *self);
} VtEntry80048650;

typedef struct {
    VtEntry80048650 *vtable;
} Inner80048650;

typedef struct {
    s32 field_0;
    s32 field_4;
    Inner80048650 *target;
    s32 state;
} Obj80048650;

void func_80048650(Obj80048650 *obj) {
    if (obj->state >= 0) {
        Inner80048650 *target = obj->target;
        VtEntry80048650 *entry = &target->vtable[1];
        entry->fn((char *)target + entry->delta);
    }
}
