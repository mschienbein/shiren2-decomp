#include "common.h"

typedef unsigned char u8;
typedef struct { u8 *buf; u32 pos; } BitStream;
void func_800A0ACC(BitStream *bs, u32 *out, u32 count){
    s32 bit;
    *out = 0;
    bit = 1;
    while (count--) {
        if ((bs->buf[bs->pos >> 3] >> (bs->pos & 7)) & 1) *out |= bit;
        bit <<= 1;
        bs->pos++;
    }
}
