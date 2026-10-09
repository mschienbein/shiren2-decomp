#include "common.h"
typedef struct { u32 w0, w1; } Gfx;
typedef struct { u32 imageRom, paletteRom; s32 format; } Image;
extern s32 D_801630B0;
extern s32 D_801630B4;
extern Gfx *(*D_801630B8)(Gfx *);
extern void *D_801630BC;
extern s32 *D_801630C0;
extern Image D_801398C0[];
extern Gfx D_01000140[];
extern Gfx *func_800551E8(Gfx *);
/* The texture argument is a CPU buffer until its address is encoded by the renderer. */
extern Gfx *func_80060048(Gfx *, void *, s32 *, s32, s32, s32, s32, s32, float, float, s32 *);
Gfx *func_80055078(Gfx *gfx)
{
    s32 image;
    void *pixels;
    s32 format;
    switch (D_801630B0) {
    case 0:
        gfx = func_800551E8(gfx);
        break;
    case 1:
        image = D_801630B4;
        if (image != 0) {
            pixels = D_801630BC;
            if (pixels != 0) {
                Gfx *command = gfx++;
                command->w0 = 0xDE000000;
                command->w1 = (u32)D_01000140;
                format = D_801398C0[image].format;
                switch (format) {
                case 0:
                case 1:
                    gfx = func_80060048(gfx, pixels, 0, 0x22, 320, 240, 0, 0, 1.0f, 1.0f, 0);
                    break;
                case 2:
                    gfx = func_80060048(gfx, pixels, D_801630C0, 0x15, 320, 240, 0, 0, 1.0f, 1.0f, 0);
                    break;
                }
            }
        }
        break;
    case 2:
        if (D_801630B8 != 0) {
            gfx = D_801630B8(gfx);
        }
        break;
    }
    return gfx;
}
