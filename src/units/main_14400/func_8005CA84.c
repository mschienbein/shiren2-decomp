#include "common.h"

typedef unsigned char u8;
typedef struct { u32 w0; u32 w1; } Gfx;
extern s32 D_8013B5F0;
extern s32 D_801E4E74;
extern Gfx D_010000D0[];
extern void func_8006A0AC(u8 *, u8 *, u8 *, u8 *);
extern s32 func_800627C4(void);

Gfx *func_8005CA84(Gfx *display) {
    u8 color[4];
    if (!D_8013B5F0) {
        return display;
    }
    func_8006A0AC(&color[0], &color[1], &color[2], &color[3]);
    switch (func_800627C4()) {
        case 2:
        default:
            color[3] = 0xC8;
            break;
        case 3:
            /* Low byte of the big-endian word stored by func_800688E8. */
            color[3] = ((u8 *)&D_801E4E74)[3];
            break;
    }
    {
        Gfx *command = display++;
        command->w0 = 0xDE000000;
        command->w1 = (u32)D_010000D0;
    }
    {
        Gfx *command = display++;
        command->w0 = 0xFA000000;
        command->w1 = (color[0] << 24) | (color[1] << 16) | (color[2] << 8) | color[3];
    }
    {
        Gfx *command = display++;
        command->w0 = 0xF64FC3BC;
        command->w1 = 0;
    }
    return display;
}
