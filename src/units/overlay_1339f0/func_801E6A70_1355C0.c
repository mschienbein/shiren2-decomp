#include "func_801E6A70_1355C0/types.h"

void func_801E6A70_1355C0(s32 glyph, u16 foreground, u16 background) {
    u16 *bitmap;
    u16 *pixels;
    s32 y;
    s32 x;
    if (glyph == 1) {
        D_801E8280_136DD0 += 5;
        return;
    }
    if ((u32)(glyph - 15) >= 134) {
        glyph = 59;
    }
    bitmap = D_801E8284_136DD4 + (glyph - 15) * 16;
    pixels = D_801E8280_136DD0;
    for (y = 0; y < 16; y++) {
        for (x = 15; x >= 0; x--) {
            if ((*bitmap >> x) & 1) {
                pixels[0] = foreground;
                pixels[641] = background;
            }
            pixels++;
        }
        pixels += 624;
        bitmap++;
    }
    D_801E8280_136DD0 += 16;
}
