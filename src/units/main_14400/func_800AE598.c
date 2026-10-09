#include "common.h"
typedef struct VTable {
    unsigned char pad_00[0x10];
    short adjustment_10, pad_12;
    s32 (*method_14)(void *self);
} VTable;
typedef struct Item80051860 {
    unsigned char kind_00;
    unsigned char pad_01[7];
    const VTable *vtable_08;
    unsigned char pad_0C;
    unsigned char flags_0D;
} Item80051860;
extern s32 func_800ACEB4(Item80051860 *item);
static __inline__ s32 marked(Item80051860 *item) {
    s32 flags = item->flags_0D;
    flags &= 2;
    return flags != 0;
}
s32 func_800AE598(Item80051860 *item) {
    if ((item->vtable_08->method_14((unsigned char *)item + item->vtable_08->adjustment_10) ^ 1) == 0) {
        if (func_800ACEB4(item) == 2) return 1;
        return item->kind_00 == 6 && marked(item);
    }
    return 0;
}
