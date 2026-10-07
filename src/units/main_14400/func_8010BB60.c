#include "common.h"

typedef unsigned char u8;
/* func_8010BC98 caps capacity at 0x10; func_8010BD00 appends at data10[countF]. */
typedef struct { u8 unk0; u8 unk1; char pad2[0xD]; u8 countF; u8 data10[0x10]; } Src;
s32 func_800A2910(u8, u8, u8 *);
s32 func_8010BB60(Src *src, u8 *dst) {
    u8 buf[8];
    s32 n = 0;
    s32 i;
    if (func_800A2910(src->unk0, src->unk1, buf)) {
        n = 1;
        *dst = src->unk1;
        dst += n;
    }
    for (i = 0; i < src->countF; i++) {
        *dst++ = src->data10[i];
    }
    return n + src->countF;
}
