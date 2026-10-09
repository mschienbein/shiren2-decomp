#include "common.h"

typedef unsigned char u8;

extern u8 D_801C51A4[];
extern const u8 D_8015488C[8];

s32 func_800A8A50(void) {
    s32 count = 0;
    s32 i = 0;
    u8 *flags = D_801C51A4;

    for (;;) {
        u8 bits;

        if (i >= 29) {
            break;
        }
        /* local-arithmetic-qualification: keep the original integer add order;
         * flags[i >> 3] reverses the addu operands at 0x800A8A6C. The offset is
         * 0..3 inside D_801C51A4, and no address crosses an integer interface. */
        bits = *(u8 *)((i >> 3) + (s32)flags);
        if (!(bits & D_8015488C[i & 7])) {
            count++;
        }
        i++;
    }
    return count;
}
