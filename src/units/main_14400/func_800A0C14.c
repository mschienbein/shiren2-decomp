#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 *buf;
    u32 pos;
} BitWriter;

void func_800A0C14(BitWriter *writer, u8 *src, s32 size, s32 count) {
    s32 i;

    size--;
    for (i = count - 1; i != -1; i--) {
        s32 index = i / 8;
        u8 *in = &src[size - index];
        s32 bit = i - index * 8;
        u8 *out = &writer->buf[writer->pos >> 3];
        s32 shift = writer->pos & 7;

        if ((*in >> bit) & 1) {
            *out |= 1 << shift;
        } else {
            *out &= ~(1 << shift);
        }
        writer->pos++;
    }
}
