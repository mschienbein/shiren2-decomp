#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
void func_8005E134(u16, s32 *, s32 *);
u16 func_80083844(void);
s32 func_8005E084(u8 *str) {
    u8 *s;
    s32 total = 0;
    s32 w, h;
    s = str;
    while (1) {
        u16 c = *s++;
        if (c == '\n' || c == 0) break;
        if ((c & 0xF0) == 0xF0) {
            c = (c << 8) + *s++;
        } else if (!(c & 0x80)) {
            continue;
        }
        func_8005E134(c, &w, &h);
        total += w + func_80083844();
    }
    return total;
}
