#include "func_801E6C80_1357D0/types.h"

extern const char D_801E81E4_136D34[];
extern void func_801E6BC4_135714(s32 x, s32 y);
extern void func_801E6B1C_13566C(const char *text, s32 length, u16 foreground, u16 background);
extern void func_801E6A70_1355C0(s32 glyph, u16 foreground, u16 background);
extern void func_801E69BC_13550C(u16 value, char *text);
extern void func_801E6C30_135780(const char *text, u16 foreground, u16 background);

void func_801E6C80_1357D0(s32 kind, s32 slot, u16 foreground, u16 label_color, u16 background) {
    char text[3];
    OvlPakState *state = &D_801E84C0_137010[slot];
    s32 y = slot * 20 + 84;
    if (slot < 16) {
        if (D_801E86C0_137210[slot] == 0) {
            func_801E6BC4_135714(126, y);
            func_801E6B1C_13566C(state->game_name, 16, foreground, background);
            if (state->ext_name[0] != 0) {
                func_801E6A70_1355C0(60, foreground, background);
                D_801E8280_136DD0 -= 8;
                func_801E6A70_1355C0(state->ext_name[0], foreground, background);
            }
            func_801E6BC4_135714(406, y);
            func_801E69BC_13550C((state->file_size + 255) >> 8, text);
            func_801E6B1C_13566C(text, 3, foreground, background);
            D_801E8280_136DD0 += D_801E7E7C_1369CC[kind];
            func_801E6B1C_13566C(D_801E7E68_1369B8[kind], D_801E7E74_1369C4[kind], label_color, background);
        }
        if (slot < 9) {
            func_801E6BC4_135714(99, y);
            func_801E6A70_1355C0(slot + 17, foreground, background);
        } else {
            func_801E6BC4_135714(92, y);
            func_801E6A70_1355C0(17, foreground, background);
            func_801E6BC4_135714(100, y);
            func_801E6A70_1355C0(slot + 7, foreground, background);
        }
        func_801E6BC4_135714(111, y);
        func_801E6A70_1355C0(62, foreground, background);
    } else {
        func_801E6BC4_135714(94, slot * 20 + 88);
        func_801E6C30_135780(D_801E81E4_136D34, foreground, background);
    }
}
