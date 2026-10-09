#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct VTable {
    u8 pad_00[0x18];
    s16 adjust_18;
    s16 pad_1A;
    void (*method_1C)(void *self);
} VTable;

typedef struct Obj {
    u8 pad_00[0x24];
    const VTable *vtable_24;
} Obj;

s32 func_80049CB4(s32 id, ...);

/* Life vtable slot 0x18 (e.g. func_800E22E0). */
void func_800F8494(Obj *self) {
    const VTable *table;

    func_80049CB4(0x70, self);
    table = self->vtable_24;
    table->method_1C((u8 *)self + table->adjust_18);
}
