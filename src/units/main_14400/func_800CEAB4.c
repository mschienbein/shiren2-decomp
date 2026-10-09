#include "common.h"
typedef unsigned char u8;
typedef struct VTable {
    u8 pad_00[0x20];
    short adjustment_20;
    short pad_22;
    s32 (*method_24)(void *self);
} VTable;
typedef struct Obj { void *pool_00; const VTable *vtable_04; } Obj;
/* Slot 4 resolves to func_800CE710; the dispatch ABI returns an int. */
s32 func_800CEAB4(Obj *self) {
    return self->vtable_04->method_24((u8 *)self + self->vtable_04->adjustment_20);
}
