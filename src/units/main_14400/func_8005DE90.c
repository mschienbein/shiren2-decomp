#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
extern void func_8005E134(s32, s32 *, s32 *);
extern char *func_80083C90(char *, char *);
extern u16 func_80083844(void);
/* Re-wraps text in place: measures each two-byte (0xFx lead) or high-bit glyph
 * and inserts a newline once the line exceeds maximum. Returns whether any
 * newline was inserted (the original epilogue moves that flag into v0). */
s32 func_8005DE90(char *text, s32 maximum) {
    char buffer[0x200];
    s32 width, height;
    s32 line_width = 0;
    s32 changed = 0;
    u8 *src = (u8 *)text;
    char *dst = buffer;
    for (;;) {
        u8 measured = 0;
        u16 ch = *src++;
        if (ch == 0) break;
        if ((ch & 0xF0) == 0xF0) {
            ch = *src++ + (ch << 8);
            measured = 1;
        } else if (ch & 0x80) {
            measured = 1;
        } else if (ch == '\n') {
            line_width = 0;
        }
        if (measured == 1) {
            func_8005E134(ch, &width, &height);
            line_width += width + func_80083844();
            if (maximum < line_width) {
                *dst++ = '\n';
                line_width = 0;
                changed = 1;
            }
        }
        if (ch >= 0x100) {
            *dst++ = ch >> 8;
            *dst++ = ch;
        } else {
            *dst++ = ch;
        }
    }
    *dst = 0;
    if (changed) func_80083C90(text, buffer);
    return changed;
}
