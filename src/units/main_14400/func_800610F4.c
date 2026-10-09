#include "common.h"
typedef struct { u32 w0, w1; } Gfx;
typedef struct { short scale[4]; short translate[4]; } Viewport;
/* The viewport starts at F0; F8 is only the translation-half splat label. */
Viewport D_8013B7F0 = {{640, 480, 511, 0}, {640, 480, 511, 0}};
extern s32 D_8013B7A0, D_8013B7A4;
extern float D_80169A3C, D_80169A40;
extern Gfx *D_80169A18[8];
Gfx *func_800610F4(Gfx *out) {
    Gfx *command;
    s32 i;
    D_8013B7F0.translate[0] = (160 - D_8013B7A0 / 2) * 4 + (s32)(D_80169A3C * 4.0f);
    D_8013B7F0.translate[1] = (120 - D_8013B7A4 / 2) * 4 + (s32)(D_80169A40 * 4.0f);
    command = out++;
    command->w0 = 0xDC080008;
    command->w1 = (u32)&D_8013B7F0;
    for (i = 0; i < 8; i++) {
        if (D_80169A18[i]) {
            command = out++;
            command->w0 = 0xDE000000;
            command->w1 = (u32)D_80169A18[i];
            D_80169A18[i] = 0;
        }
    }
    return out;
}
