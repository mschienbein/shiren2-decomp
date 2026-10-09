#include "common.h"
/* Slot 0x90 in the entity vtable dispatches func_800F212C. */
typedef struct { unsigned char pad0[0x90]; short adjustment90; short pad92; s32 (*invoke94)(void *, s32, s32, unsigned char, s32); } VTable;
typedef struct { unsigned char pad0[0x24]; VTable *vtable24; } Object;
s32 func_800E2F40(Object *object) {
    VTable *table = object->vtable24;
    return table->invoke94((unsigned char *)object + table->adjustment90, 1, 5, 0, 0);
}
