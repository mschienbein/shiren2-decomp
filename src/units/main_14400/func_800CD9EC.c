#include "common.h"
typedef unsigned char u8;
/* List vtable slots (D_80154390): +0x24 func_800CE710 count, +0x3C func_800CE7A0 getter. */
typedef struct { u8 pad0[0x20]; short adjust_20; short pad22; s32 (*count_24)(void *); u8 pad28[0x10]; short adjust_38; short pad3A; void *(*get_3C)(void *, u32); } VTable;
typedef struct { void *pool_0; VTable *field_4; } Object;
u8 *func_800CD9EC(Object *self, u8 value) {
    VTable *table = self->field_4;
    s32 target = value;
    s32 index = table->count_24((u8 *)self + table->adjust_20) - 1;
    for (;;) {
        u8 *entry;
        if (index < 0) return 0;
        table = self->field_4;
        entry = table->get_3C((u8 *)self + table->adjust_38, (u32)index);
        index--;
        if (*entry == target) return entry;
    }
}
