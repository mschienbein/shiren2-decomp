#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef float f32;
typedef struct { u32 w0; u32 w1; } Gfx;
extern Gfx *D_801DEAB0;
/* CPU image buffers filled by func_80069A68. */
extern void *D_8013C760;
extern u8 *D_8013C764;
extern Gfx D_010000B8[];
extern Gfx D_8013C770[];
extern Gfx D_8013C790[];
/* Complete 16-byte display list (.data 0x8013C7D0: G_GEOMETRYMODE, G_ENDDL) and complete
 * 32-byte CI4 palette (.data 0x8013C7E0..0x8013C7FF), loaded by func_80060048 as a 16-entry
 * TLUT (0x80060144 count field 0x703C000). */
extern s32 D_8013C7D0[4];
extern s32 D_8013C7E0[8];
void func_8006126C(f32 x, f32 y);
Gfx *func_80060048(Gfx *gfx, void *image, s32 *palette, s32 format, s32 width, s32 height, s32 x, s32 y, f32 scaleX, f32 scaleY, s32 *extra);
void func_80069BFC(void) {
    Gfx *g;
    func_8006126C(0.0f, 0.0f);
    g = D_801DEAB0++;
    g->w0 = 0xDE000000;
    g->w1 = (u32)D_010000B8;
    g = D_801DEAB0++;
    g->w0 = 0xDE000000;
    g->w1 = (u32)D_8013C770;
    D_801DEAB0 = func_80060048(D_801DEAB0, D_8013C760, 0, 0x22, 320, 240, 0, 0, 1.0f, 1.0f, 0);
    {
        Gfx *g2 = D_801DEAB0++;
        g2->w0 = 0xDE000000;
        g2->w1 = (u32)D_8013C790;
    }
    D_801DEAB0 = func_80060048(D_801DEAB0, D_8013C764, D_8013C7E0, 4, 320, 240, 0, 0, 1.0f, 1.0f, D_8013C7D0);
    {
        Gfx *g3 = D_801DEAB0++;
        g3->w0 = 0xE2001D00;
        g3->w1 = 0;
    }
}
