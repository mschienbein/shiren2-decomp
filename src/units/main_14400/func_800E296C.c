#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct VTable {
    u8 pad_00[0x90];
    s16 adjust_90;
    s16 pad_92;
    s32 (*method_94)(void *self, s32 mode, s32 value, u8 byte, s32 extra);
} VTable;

typedef struct Obj {
    u8 pad_00[0x24];
    const VTable *vtable_24;
} Obj;

/* Life vtable slot 0x90 (e.g. func_800E115C / func_800F212C). */
s32 func_800E296C(Obj *self) {
    const VTable *table = self->vtable_24;
    return table->method_94((u8 *)self + table->adjust_90, 0, 15, 255, 0);
}
