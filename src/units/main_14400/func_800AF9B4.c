#include "common.h"

typedef unsigned char u8;

/* Partial view: bit array pointer at 0x04, bit count at 0x0C. */
typedef struct Bits800AF9B4 {
    u32 opaque_00;
    u8 *bits_04;
    u32 opaque_08;
    s32 count_0C;
} Bits800AF9B4;

extern const unsigned char D_8015488C[8]; /* single-bit masks indexed by bit position */

/* True when at least two bits of the set are clear: find two clear bits in turn,
 * giving up (none) once the scan reaches the end. */
s32 func_800AF9B4(void *object)
{
    Bits800AF9B4 *set = object;
    s32 clear = 0;
    s32 i = 0;
    s32 count = set->count_0C;

    for (; clear < 2; clear++) {
        do {
            if (i >= count) {
                goto none;
            }
        } while (set->bits_04[i >> 3] & D_8015488C[i++ & 7]);
    }
    return 1;
none:
    return 0;
}
