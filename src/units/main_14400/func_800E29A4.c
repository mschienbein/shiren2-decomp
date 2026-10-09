#include "common.h"
typedef unsigned char u8;
typedef struct VTable {
    u8 pad_00[0x90];
    short adjustment_90;
    short pad_92;
    s32 (*method_94)(void *self, s32 mode, s32 value, u8 byte, s32 extra);
} VTable;
typedef struct Obj { u8 pad_00[0x24]; const VTable *vtable_24; } Obj;
/* Life vtable slot 18 resolves to func_800E115C. */
s32 func_800E29A4(Obj *self) {
    return self->vtable_24->method_94((u8 *)self + self->vtable_24->adjustment_90, 0, 15, 254, 0);
}
