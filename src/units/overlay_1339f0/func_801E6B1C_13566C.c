#include "func_801E6B1C_13566C/types.h"

extern void func_801E6A70_1355C0(s32 glyph, u16 foreground, u16 background);

void func_801E6B1C_13566C(const char *text, s32 length, u16 foreground, u16 background) {
    for (length--; length != -1; length--) {
        if (*text == 0) {
            break;
        }
        func_801E6A70_1355C0(*text++, foreground, background);
    }
}
