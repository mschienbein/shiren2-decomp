#include "common.h"
typedef struct {
    unsigned char pad_00[8]; short adjustment_08; unsigned short pad_0A;
    void (*destroy_0C)(void *, s32);
} VTable800AACE0;
typedef struct {
    unsigned char kind; unsigned char pad_01[7]; VTable800AACE0 *vtable_08;
} Item800AACE0;
extern unsigned char D_80142DE0[];
extern void *func_800AB16C(void *table, unsigned char kind, s32 arg);
extern void *func_800AC244(unsigned char id);
static __inline__ s32 accepts_item(Item800AACE0 *item) {
    return item->kind != 0x11 && item->kind != 0xA;
}
Item800AACE0 *func_800AACE0(s32 argument) {
    s32 remaining = 100;
    for (;;) {
        Item800AACE0 *item;
        if (remaining-- <= 0) {
            break;
        }
        item = func_800AB16C(D_80142DE0, 0, argument);
        if (item == 0 || accepts_item(item)) {
            return item;
        }
        item->vtable_08->destroy_0C((unsigned char *)item + item->vtable_08->adjustment_08, 3);
    }
    return func_800AC244(0x16);
}
