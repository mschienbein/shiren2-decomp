#include "common.h"
/* Slot 0x94: 800E115C / 800F212C, integer status dispatcher. */
typedef struct {
    unsigned char pad00[0x90]; short adjustment; unsigned short pad92;
    s32 (*dispatch)(void *, s32, s32, unsigned char, s32);
} Vtable;
typedef struct { unsigned char pad00[0x24]; Vtable *vtable; } Object;
s32 func_800E3148(Object *object) {
    return object->vtable->dispatch((unsigned char *)object + object->vtable->adjustment, 0, 2, 0xFE, 0);
}
