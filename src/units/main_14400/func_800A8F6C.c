#include "common.h"

extern unsigned char D_801C51A4[];
extern unsigned char D_8015488C[];
static inline s32 testbit(unsigned char *bits, s32 n) { return (bits[n >> 3] & D_8015488C[n & 7]) != 0; }
s32 func_800A8F6C(s32 *p) {
    for (;;) {
        s32 v = *p;
        if (v >= 0x1D) break;
        if (testbit(D_801C51A4, v)) return 1;
        *p = v + 1;
    }
    return *p == 0x1D;
}
