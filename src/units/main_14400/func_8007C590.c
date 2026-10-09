#include "common.h"
typedef unsigned char u8;
typedef struct { u32 w0, w1; } Gfx;
typedef struct { u8 r, g, b, a; } Color;
extern float D_8013B7A8, D_8013B7AC, D_8013B7B0, D_8013B7B4;
extern s32 D_8013DE80, D_8013DE84, D_8013DE88, D_8013DE8C, D_8013DE90, D_8013DE94, D_8013DE98, D_8013DEA0;
extern Color D_8013DEA4[21], D_8013DEF8[21];
extern u8 D_8013D960[32], D_8013D980[1280];
extern Gfx *func_8007CE28(Gfx *, s32, s32);
extern Gfx *func_8007CA78(Gfx *);
extern Gfx *func_8007D3C0(Gfx *, void *, s32);
extern s32 func_80041E50(void);
extern Gfx *func_8007CD30(Gfx *, s32, s32, s32);
extern Gfx *func_8007D154(Gfx *, s32, s32, s32, s32, s32, s32);
extern Gfx *func_8007CCBC(Gfx *);
Gfx *func_8007C590(Gfx *gfx) {
    if (!D_8013DE98) return gfx;
    {
        Gfx *command = gfx++;
        command->w0 = 0xED000000 | (((s32)(D_8013B7A8 * 4.0f) & 0xFFF) << 12) | ((s32)(D_8013B7AC * 4.0f) & 0xFFF);
        command->w1 = (((s32)((D_8013B7B0 - 4.0f) * 4.0f) & 0xFFF) << 12) | ((s32)(D_8013B7B4 * 4.0f) & 0xFFF);
    }
    gfx = func_8007CE28(gfx, D_8013DE88, D_8013DE8C);
    gfx = func_8007CA78(gfx);
    { Gfx *command = gfx++; command->w0 = 0xFD48004F; command->w1 = (u32)D_8013D980; }
    { Gfx *command = gfx++; command->w0 = 0xF5481400; command->w1 = 0x07080200; }
    { Gfx *command = gfx++; command->w0 = 0xE6000000; command->w1 = 0; }
    { Gfx *command = gfx++; command->w0 = 0xF4000000; command->w1 = 0x0713E03C; }
    { Gfx *command = gfx++; command->w0 = 0xE7000000; command->w1 = 0x0; }
    { Gfx *command = gfx++; command->w0 = 0xF5401400; command->w1 = 0x00080200; }
    { Gfx *command = gfx++; command->w0 = 0xF2000000; command->w1 = 0x0027C03C; }
    gfx = func_8007D3C0(gfx, D_8013D960, 0);
    { Gfx *command = gfx++; command->w0 = 0xFA000000; command->w1 = 0xFFFFFFFF; }
    if (D_8013DE80 && !(u8)func_80041E50()) gfx = func_8007CD30(gfx, D_8013DE80, 0x2C, 2);
    gfx = func_8007CD30(gfx, D_8013DE84, 0x64, 2);
    gfx = func_8007CD30(gfx, D_8013DE88, 0x8C, 3);
    gfx = func_8007CD30(gfx, D_8013DE8C, 0xAC, 3);
    if (D_8013DE90 >= 0) gfx = func_8007CD30(gfx, D_8013DE90, 0xCC, 7);
    if (!D_8013DE94) {
        Gfx *command = gfx++; command->w0 = 0xFA000000;
        command->w1 = ((u32)D_8013DEF8[0].r << 24) | (D_8013DEF8[0].g << 16) | (D_8013DEF8[0].b << 8) | D_8013DEF8[0].a;
    } else {
        Gfx *command = gfx++; command->w0 = 0xFA000000;
        command->w1 = ((u32)D_8013DEA4[D_8013DEA0].r << 24) | (D_8013DEA4[D_8013DEA0].g << 16) | (D_8013DEA4[D_8013DEA0].b << 8) | D_8013DEA4[D_8013DEA0].a;
    }
    if (D_8013DE80 && !(u8)func_80041E50()) gfx = func_8007D154(gfx, 0x78, 0, 0x10, 0x10, 0x3C, 0x14);
    gfx = func_8007D154(gfx, 0x68, 0, 0x10, 0x10, 0x54, 0x14);
    gfx = func_8007D154(gfx, 0x58, 0, 0x10, 0x10, 0x7C, 0x14);
    gfx = func_8007D154(gfx, 0x50, 0, 0x08, 0x10, 0xA4, 0x14);
    gfx = func_8007D154(gfx, 0x88, 0, 0x10, 0x10, 0x104, 0x14);
    {
        Gfx *command = gfx++;
        command->w0 = 0xED000000 | (((s32)(D_8013B7A8 * 4.0f) & 0xFFF) << 12) | ((s32)(D_8013B7AC * 4.0f) & 0xFFF);
        command->w1 = (((s32)(D_8013B7B0 * 4.0f) & 0xFFF) << 12) | ((s32)(D_8013B7B4 * 4.0f) & 0xFFF);
    }
    return func_8007CCBC(gfx);
}
