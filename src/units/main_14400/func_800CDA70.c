#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Entry800CDA70 {
    u8 field_00;
    u8 kind_01;
} Entry800CDA70;

/* List interface (base vtable D_80154300, e.g. D_80154390: func_800CE710 / func_800CE7A0). */
typedef struct ListVTable {
    u8 pad_00[0x20];
    s16 adjust_20;
    s16 pad_22;
    s32 (*count_24)(void *self);
    u8 pad_28[0x38 - 0x28];
    s16 adjust_38;
    s16 pad_3A;
    void *(*get_3C)(void *self, u32 index);
} ListVTable;

typedef struct List800CDA70 {
    void *pool_00;
    const ListVTable *vtable_04;
} List800CDA70;

/* Last entry of `list` whose kind_01 equals `kind`, or 0. */
Entry800CDA70 *func_800CDA70(List800CDA70 *list, u8 kind) {
    s32 i = list->vtable_04->count_24((u8 *)list + list->vtable_04->adjust_20) - 1;

    while (1) {
        Entry800CDA70 *entry;

        if (i < 0) {
            return 0;
        }
        entry = list->vtable_04->get_3C((u8 *)list + list->vtable_04->adjust_38, (u32)i);
        i--;
        if (entry->kind_01 == kind) {
            return entry;
        }
    }
}
