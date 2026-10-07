#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
void func_80060BC8(u16 *dst, u16 *src, u32 *w, u32 *h, u8 sx, u8 sy) {
    u32 x, y;
    u32 cols = 0, rows = 0;
    u16 *p;
    for (y = 0; y < *h; y += sy, rows++) {
        p = &src[*w * y];
        cols = 0;
        for (x = 0; x < *w; dst++) {
            x += sx;
            cols++;
            *dst = *p;
            p += sx;
        }
    }
    *w = cols;
    *h = rows;
}
