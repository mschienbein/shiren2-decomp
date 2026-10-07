#include "common.h"

typedef unsigned char u8;
/* D_801C36EC: 29 unit slots of 0xE4 bytes plus the fallback slot at index 29 (0x801C50C0);
 * D_801C51A4 is the occupancy bitmap of slots 0..28. */
typedef struct { char data[0xE4]; } Slot;
extern s32 D_80142B00;
extern Slot D_801C36EC[30];
extern u8 D_801C51A4[];
extern u8 D_8015488C[];
Slot *func_800A88D8(void) {
    s32 idx = D_80142B00;
    s32 i;
    if (idx >= 0) {
        return &D_801C36EC[idx];
    }
    i = 0;
    for (;;) {
        u8 *bits;
        if (i >= 0x1D) {
            break;
        }
        bits = &D_801C51A4[i >> 3];
        if (!(*bits & D_8015488C[i & 7])) {
            *bits |= D_8015488C[i & 7];
            return &D_801C36EC[i];
        }
        i++;
    }
    return &D_801C36EC[29];
}
