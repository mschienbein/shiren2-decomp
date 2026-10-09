#include "common.h"

typedef struct {
    s32 fields_00[2];
    short adjust_08;
    short field_0A;
    void (*method_0C)(void *, s32);
} MethodTable;

/* 29 unit slots plus the fallback slot, each 0xE4 bytes. The unused prefix
 * includes byte fields +0xA/+0x1E (lbu at 0x800A8AEC/0x800A8B00). */
typedef struct {
    unsigned char pad_00[0x24];
    MethodTable *table_24;
    unsigned char fields_28[0xBC];
} Object;

extern const unsigned char D_8015488C[8];
extern unsigned char D_801C51A4[];
extern Object D_801C36EC[30];

/* Bit i of the byte-packed occupancy set (D_8015488C holds the per-bit masks). */
static inline s32 test_bit(unsigned char *bits, s32 i) {
    return bits[i >> 3] & D_8015488C[i & 7];
}

void func_800A8774(void) {
    s32 i = 0;
    for (;;) {
        Object *object;
        if (i >= 29) {
            return;
        }
        object = &D_801C36EC[i];
        if (test_bit(D_801C51A4, i) && object != 0) {
            MethodTable *table = object->table_24;
            table->method_0C((unsigned char *)object + table->adjust_08, 3);
        }
        ++i;
    }
}
