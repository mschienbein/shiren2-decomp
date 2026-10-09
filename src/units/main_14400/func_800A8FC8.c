#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x1E];
    u8 unk1E;
    u8 pad1F[0xE4 - 0x1F];
} Entry_801C36EC;

extern u8 D_801C51A4[];
extern const unsigned char D_8015488C[8];
extern Entry_801C36EC D_801C36EC[];
extern u8 D_801C35E0[];

s32 func_800A8FC8(s32 *index, s32 mask) {
    u8 *flags = D_801C51A4;
    Entry_801C36EC *entries = D_801C36EC;

    for (;;) {
        s32 i = *index;

        if (i >= 29) {
            break;
        }
        /* local-arithmetic-qualification: both pointer spellings emit base+index,
         * reversing the original add operands; this local 32-bit calculation
         * still selects the same byte of the complete occupancy array. */
        if ((*(u8 *)((i >> 3) + (u32)flags) & D_8015488C[i & 7]) && (entries[i].unk1E & mask)) {
            return 1;
        }
        (*index)++;
    }
    if (*index == 29 && (((Entry_801C36EC *)D_801C35E0)->unk1E & mask)) {
        return 1;
    }
    return 0;
}
