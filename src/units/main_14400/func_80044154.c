#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    s32 pos_0;
    s32 end_4;
    u8 pad8[0xC];
    void *error_14;
    u8 pad18[8];
    u8 buf_20[0x20];
    s32 base_40;
} Stream80044154;
extern s32 D_80138B00;
extern char D_8014A82C[];
void func_80043ED8(Stream80044154 *s);
void *func_80032D94(void *dst, const void *src, u32 count);

void func_80044154(Stream80044154 *s, s32 size, u8 *dst) {
    s32 offset;
    u32 chunk;

    if (D_80138B00 != 0) {
        return;
    }
    if (s->end_4 - s->pos_0 < size) {
        s->error_14 = D_8014A82C;
        return;
    }
    for (;;) {
        if (size == 0) {
            return;
        }
        func_80043ED8(s);
        offset = s->pos_0 - s->base_40;
        chunk = 0x20 - offset;
        if ((u32)size < chunk) {
            chunk = size;
        }
        /* &s->buf_20[offset], summed as integers (pointer spellings reorder the addu) */
        func_80032D94(dst, (u8 *)(offset + (u32)s) + 0x20, chunk);
        size -= chunk;
        dst += chunk;
        s->pos_0 += chunk;
    }
}
