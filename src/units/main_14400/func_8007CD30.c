#include "common.h"
typedef struct { u32 w0; u32 w1; } Gfx;
extern char D_8014D24C[];
s32 func_800327C0(char *buf, const char *fmt, ...);
Gfx *func_8007D154(Gfx *gfx, s32 u, s32 v, s32 w, s32 h, s32 x, s32 y);

Gfx *func_8007CD30(Gfx *gfx, s32 value, s32 x, s32 digits) {
    char buf[16];
    s32 i;
    func_800327C0(buf, D_8014D24C, value);
    for (i = 8 - digits; i < 8; i++) {
        if (buf[i] != ' ') {
            gfx = func_8007D154(gfx, (buf[i] - '0') * 8, 0, 8, 16, x + (i - (8 - digits)) * 8, 20);
        }
    }
    return gfx;
}
