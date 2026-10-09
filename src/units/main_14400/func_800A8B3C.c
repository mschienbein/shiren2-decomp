#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x1E];
    u8 flags;
    u8 pad1F[0xC5];
} Unit;

extern Unit D_801C36EC[];
extern u8 D_801C51A4[];
extern const unsigned char D_8015488C[8];
u16 func_800E08B0(Unit *);

s32 func_800A8B3C(void) {
    s32 count = 0;
    s32 i = 0;
    u8 *flags = D_801C51A4;
    Unit *unit = D_801C36EC;

    while (1) {
        if (i >= 29) {
            break;
        }
        /* local-arithmetic-qualification: GCC reverses the original add operands
         * for both base[index] and index+base; this local address stays within
         * the same four occupancy bytes used for the 29-record pool. */
        if (*(u8 *)((i >> 3) + (u32)flags) & D_8015488C[i & 7]) {
            s32 active = 0;

            if (unit->flags & 0x7C) {
                active = func_800E08B0(unit) != 0;
            }
            if (active) {
                count++;
            }
        }
        unit++;
        i++;
    }
    return count;
}
