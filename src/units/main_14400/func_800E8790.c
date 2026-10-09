#include "common.h"

typedef unsigned char u8;

typedef struct {
    u32 bits;
    s32 unk4;
} Flags800E8790;

extern u8 func_800E8B10(void *obj, u8 **out);

void func_800E8790(void *obj, Flags800E8790 *flags) {
    u8 *items[2];
    s32 i;

    i = func_800E8B10(obj, items);
    for (;;) {
        u8 *item;

        if (--i == -1) {
            break;
        }
        item = items[i];
        switch (item[1]) {
        case 0x7D:
            flags->bits |= 0x80;
            break;
        case 0x8B:
            flags->bits |= 0x200000;
            break;
        case 0x87:
            flags->bits |= 0x20;
            break;
        case 0x86:
            flags->bits |= 0x800;
            break;
        case 0x89:
            flags->bits |= 0x4;
            break;
        case 0x82:
            if ((flags->bits >> 18) & 1) {
                flags->bits &= ~0x40000;
            } else {
                flags->bits &= ~0x40000;
                flags->bits |= 0x20000;
            }
            break;
        case 0x83:
            if ((flags->bits >> 17) & 1) {
                flags->bits &= ~0x20000;
            } else {
                flags->bits &= ~0x20000;
                flags->bits |= 0x40000;
            }
            break;
        case 0x7C:
            flags->bits |= 0x1;
            break;
        case 0x85:
            flags->bits |= 0x1000000;
            break;
        case 0x84:
            flags->bits |= 0x10000;
            break;
        case 0x81:
            flags->bits |= 0x2;
            break;
        case 0x8A:
            flags->bits |= 0x800000;
            break;
        case 0x8D:
            flags->bits |= 0x80000;
            break;
        }
    }
}
