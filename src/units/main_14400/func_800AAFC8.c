#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
extern u16 D_80142B16;
extern u8 D_80147620[];
extern u8 D_80142D28[];
u16 func_800C58DC(void *rng, u16 range);
void *func_800AADB4(u8 id, s32 b);
void *func_800AAFC8(void) {
    u16 roll = func_800C58DC(D_80147620, D_80142B16 - 1);
    u8 *p = D_80142D28;
    while (p[0] != 0) {
        if (roll < p[1]) return func_800AADB4(p[0], 0);
        roll -= p[1];
        p += 2;
    }
    return 0;
}
