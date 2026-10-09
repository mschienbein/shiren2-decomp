#include "common.h"

typedef unsigned char u8;
typedef float f32;
typedef struct Gfx {
    u32 w0;
    u32 w1;
} Gfx;

extern u8 D_8013DF70;
/* Owned by canonical func_8007D58C.c: loaded flag and the image/palette
 * buffers filled by func_8007D480. */
extern s32 D_8013DF74;
extern void *D_8013DF78;
extern void *D_8013DF7C;
extern void *D_8013DF80;
extern void *D_8013DF84;
/* Static display lists called before each image. */
extern Gfx D_8013DF88[];
extern Gfx D_8013DFA8[];

Gfx *func_80060048(Gfx *gfx, const void *image, const void *palette, s32 format, s32 width, s32 height, s32 x, s32 y, float scaleX, float scaleY, const Gfx *extra);

/* Draws the two images loaded by func_8007D480 (formats 20 and 21) at
 * (104, 60), 268x170, behind the static display lists D_8013DFA8/D_8013DF88,
 * with primitive alpha D_8013DF70. */
Gfx *func_8007D5B8(Gfx *gfx) {
    Gfx *cmd;
    s32 y, width, height, x;
    f32 scale;

    if (D_8013DF74 == 0 || D_8013DF70 == 0) {
        return gfx;
    }

    y = 60;
    width = 268;
    height = 170;
    x = 104;
    scale = 1.0f;

    cmd = gfx++;
    cmd->w0 = 0xDE000000;
    cmd->w1 = (u32)D_8013DFA8;
    /* Primitive color: white with the current fade alpha.
       ODD_C: written in place before advancing gfx (the other commands claim a
       slot first); this keeps ROM's single cursor register for both commands. */
    gfx->w0 = 0xFA000000;
    gfx->w1 = 0xFFFFFF00 | D_8013DF70;
    gfx++;
    gfx = func_80060048(gfx, D_8013DF80, D_8013DF84, 20, width, height, x, y, scale, scale, 0);

    /* Fully opaque uses the alternate display list for the second image.
       ODD_C: each arm makes its own image call; GCC cross-jumps the identical
       call tails, leaving the per-arm cursor advance that ROM has. */
    if (D_8013DF70 == 255) {
        cmd = gfx++;
        cmd->w0 = 0xDE000000;
        cmd->w1 = (u32)D_8013DF88;
        gfx = func_80060048(gfx, D_8013DF78, D_8013DF7C, 21, width, height, x, y, scale, scale, 0);
    } else {
        cmd = gfx++;
        cmd->w0 = 0xDE000000;
        cmd->w1 = (u32)D_8013DFA8;
        gfx = func_80060048(gfx, D_8013DF78, D_8013DF7C, 21, width, height, x, y, scale, scale, 0);
    }
    return gfx;
}
