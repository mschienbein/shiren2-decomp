#include "func_801E6EC4_135A14/types.h"

extern void func_801E69BC_13550C(u16 value, char *text);
extern void func_801E6BC4_135714(s32 x, s32 y);
extern u16 *func_801E6BA0_1356F0(s32 x, s32 y);
extern void func_801E6B1C_13566C(const char *text, s32 length, u16 foreground, u16 background);

void func_801E6EC4_135A14(u16 value) {
    char text[3];
    s32 y;
    s32 x;
    func_801E69BC_13550C(value, text);
    func_801E6BC4_135714(406, 408);
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 48; x++) {
            s32 offset = y * 640 + x;
            u16 *pixels = func_801E6BA0_1356F0(406, 408);
            pixels[offset] = 0;
        }
    }
    func_801E6B1C_13566C(text, 3, 0xFFFF, 1);
}
