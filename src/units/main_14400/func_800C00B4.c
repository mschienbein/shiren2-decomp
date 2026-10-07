#include "common.h"

typedef unsigned char u8;

s32 func_800A32D8(void *a, void *b);
s32 func_800C00B4(u8 *self, void *rect) {
    s32 i = 0;
    s32 offset = 0;

    while (1) {
        if (i >= *(s32 *)(self + 0x3DC)) {
            break;
        }
        if (func_800A32D8(rect, (u8 *)(offset + (s32)self) + 0x404)) {
            return 1;
        }
        offset += 0x14;
        i++;
    }
    return 0;
}
