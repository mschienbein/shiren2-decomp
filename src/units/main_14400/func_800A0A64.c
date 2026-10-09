#include "common.h"
typedef unsigned char u8;
typedef struct BitWriter800CB618 BitWriter800CB618;
typedef BitWriter800CB618 BitWriter800A09E8;
extern void func_800A09E8(BitWriter800A09E8 *bw, u32 bits, u32 count);
void func_800A0A64(BitWriter800CB618 *w, u8 *bytes, s32 count) {
    for (;;) {
        --count;
        if (count == -1) return;
        func_800A09E8(w, *bytes++, 8);
    }
}
