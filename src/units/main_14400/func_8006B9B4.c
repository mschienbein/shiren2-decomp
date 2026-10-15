#include "common.h"

extern unsigned char D_8019F8C0[];

void func_8006B9B4(s32 x, s32 y, s32 color, unsigned char *text) {
    s32 newline = 0;
    s32 keep;
    unsigned char attr;

    if (x < 0) x = 0; else if (x >= 36) x = 35;
    if (y < 0) y = 0; else if (y >= 26) y = 25;
    keep = 0;
    if (color >= 0) attr = color << 6;
    else { keep = 0xC0; attr = 0; }
    while (*text) {
        switch (*text) {
        case 9:
            x = (x + 8) & -8;
            if (x >= 36) newline = 1;
            break;
        case 10:
            newline = 1;
            break;
        default: {
            s32 ch = *text;
            unsigned char glyph = ch - 0x40;
            if ((u32)(ch - 0x61) >= 26) {
                s32 code = ch - 0x20;
                glyph = code;
                if ((unsigned char)code >= 64) glyph = 63;
            }
            {
                s32 offset = y * 36 + x++;
                D_8019F8C0[offset] = attr | (glyph | (D_8019F8C0[offset] & keep));
            }
            if (x >= 36) newline = 1;
        }
            break;
        }
        if (newline) {
            newline = 0;
            x = 0;
            if (++y >= 26) y = 0;
        }
        text++;
    }
}
