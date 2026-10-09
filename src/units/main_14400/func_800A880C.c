#include "common.h"

typedef struct {
    s32 fields_00[2];
    short adjust_08;
    short field_0A;
    void (*method_0C)(void *, s32);
} MethodTable;

typedef struct {
    s32 field_00;
    s32 field_04;
} Component;

typedef struct {
    unsigned char fields_00[0x1E];
    unsigned char flags_1E;
    unsigned char field_1F;
    s32 field_20;
    MethodTable *table_24;
    unsigned char fields_28[0x5C];
    Component component_84;
    unsigned char fields_8C[0x58];
} Object;

extern const unsigned char D_8015488C[8];
extern unsigned char D_801C51A4[];
extern Object D_801C36EC[];

/* Bit i of the byte-packed occupancy set (D_8015488C holds the per-bit masks). */
static inline s32 test_bit(unsigned char *bits, s32 i) {
    return bits[i >> 3] & D_8015488C[i & 7];
}

void func_800A880C(void) {
    s32 i = 0;
    for (;;) {
        Object *object;
        s32 skip;
        if (i >= 29) {
            return;
        }
        object = &D_801C36EC[i];
        skip = 0;
        if (test_bit(D_801C51A4, i)) {
            Object *current = object;
            if ((object->flags_1E >> 3) & 1) {
                Component *component = object ? &object->component_84 : 0;
                if (component->field_04 != 0) {
                    skip = 1;
                }
            }
            if (!skip && current != 0) {
                MethodTable *table = current->table_24;
                table->method_0C((unsigned char *)current + table->adjust_08, 3);
            }
        }
        ++i;
    }
}
