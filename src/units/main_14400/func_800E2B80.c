#include "common.h"

typedef unsigned char u8;

/* Slot 0x94 targets func_800E115C and the derived func_800F212C. */
typedef struct VTable800E2B80 {
    u8 pad_00[0x90];
    short delta_90;
    short index_92;
    s32 (*method_94)(void *self, s32 mode, s32 value, u8 byte, s32 extra);
} VTable800E2B80;

typedef struct Obj800E2B80 {
    u8 pad_00[0x24];
    VTable800E2B80 *vtable_24;
} Obj800E2B80;

s32 func_800E2B80(Obj800E2B80 *obj)
{
    VTable800E2B80 *table = obj->vtable_24;

    return table->method_94((u8 *)obj + table->delta_90, 0, 0xC, 0xFE, 0);
}
