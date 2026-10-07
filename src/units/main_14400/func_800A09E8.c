#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 *buf;
    u32 pos;
} BitWriter800A09E8;

void func_800A09E8(BitWriter800A09E8 *bw, u32 bits, u32 count) {
    while (count-- != 0) {
        u8 *p = bw->buf + (bw->pos >> 3);
        s32 shift = bw->pos & 7;
        s32 clear = shift;

        if (bits & 1) {
            *p |= 1 << shift;
        } else {
            *p &= ~(1 << clear);
        }
        bits >>= 1;
        bw->pos++;
    }
}
