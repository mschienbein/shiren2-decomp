#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef float f32;
typedef struct { s32 m[4][4]; } Mtx800706CC;
typedef struct {
    u8 kind_0;
    u8 kind_1;
    u8 pad2[2];
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    s32 alpha_18;
    u8 field_1C;
    u8 field_1D;
    u8 field_1E;
    u8 field_1F;
    u8 field_20;
    u8 field_21;
    u8 field_22;
    u8 field_23;
    u8 field_24;
    u8 field_25;
    u8 field_26;
    u8 field_27;
    u8 field_28;
    u8 field_29;
    u8 field_2A;
    u8 field_2B;
    Mtx800706CC *mtx_2C;
    void *field_30;
    u8 pad34[4];
    u8 field_38;
    u8 field_39;
    u8 pad3A[2];
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    s32 mode_48;
    void *field_4C;
    u8 field_50;
    u8 field_51;
    u8 field_52;
} Desc800706CC;
Mtx800706CC *func_80070660(u32 count);
void func_80059B58(Mtx800706CC **a, Mtx800706CC **out, Mtx800706CC **b);
void func_80030FD0(Mtx800706CC *mtx, f32 a, f32 b, f32 c, f32 d);
void func_8002CCC0(Mtx800706CC *a, Mtx800706CC *b, Mtx800706CC *out);
void func_8002CFB0(Mtx800706CC *src, f32 a, f32 b, f32 c, f32 *outA, f32 *outB, f32 *outC);
void func_80033A20(Mtx800706CC *mtx, f32 a, f32 b, f32 c);
void func_80031220(Mtx800706CC *mtx, f32 x, f32 y, f32 z);

s32 func_800706CC(Desc800706CC *desc, s32 flag, Mtx800706CC *src, f32 dist, f32 angle, f32 height, f32 scale,
                  f32 scaleMul, s32 alpha, s32 mode) {
    Mtx800706CC rot;
    Mtx800706CC *camera;
    f32 outA;
    f32 outB;
    f32 outC;
    Mtx800706CC *mtx;
    s32 maxLevel;
    s32 level;
    s32 result = 0;

    do { /* ODD_C: single-pass block grouping the matrix allocation and the whole
          * descriptor build so allocation failure exits through break; this
          * early-exit form also shapes scheduling: the stack-passed parameters
          * are loaded into saved registers at entry, as in the original. */
        mtx = func_80070660(1);
        if (mtx == 0) {
            result = -1;
            break;
        }
        if (mode != -1) {
            func_80059B58(0, &camera, 0);
            func_80030FD0(mtx, -90.0f, 1.0f, 0.0f, 0.0f);
            func_8002CCC0(mtx, camera, mtx);
            maxLevel = 0x80;
        } else {
            func_80030FD0(mtx, -90.0f, 1.0f, 0.0f, 0.0f);
            maxLevel = 0xC0;
        }
        func_8002CFB0(src, 0.0f, height, 0.0f, &outA, &outB, &outC);
        func_80033A20(&rot, outA, angle, outC);
        func_8002CCC0(mtx, &rot, mtx);
        scale *= scaleMul;
        func_80031220(&rot, scale, scale, scale);
        func_8002CCC0(&rot, mtx, mtx);
        level = maxLevel;
        if (!(dist <= 16.0f)) {
            level = 0;
            if (outB >= 150.0f) {
                result = -1;
            } else {
                level = (150.0f - dist) / 134.0f * maxLevel;
            }
        }
        desc->kind_0 = 1;
        desc->kind_1 = 9;
        desc->mtx_2C = mtx;
        desc->field_30 = 0;
        desc->field_8 = 0x200005;
        level = (f32)(level * alpha) / 255.0f;
        desc->field_4 = 0x100000;
        /* Both render-mode words are written per variant. */
        if (flag == 0) {
            desc->field_C = 0x0C080000;
            desc->field_10 = 0x104DD8;
        } else {
            desc->field_C = 0x0C080000;
            desc->field_10 = 0x1049D8;
        }
        desc->alpha_18 = (u8)level;
        desc->field_14 = 0;
        desc->field_1C = 3;
        desc->field_1D = 5;
        desc->field_1E = 1;
        desc->field_1F = 5;
        desc->field_20 = 1;
        desc->field_21 = 7;
        desc->field_22 = 5;
        desc->field_23 = 7;
        desc->field_24 = 0x1F;
        desc->field_25 = 0x1F;
        desc->field_26 = 0x1F;
        desc->field_27 = 0;
        desc->field_28 = 7;
        desc->field_29 = 7;
        desc->field_2A = 7;
        desc->field_2B = 0;
        if (mode != -1) {
            desc->mode_48 = mode;
        } else {
            desc->mode_48 = 2;
        }
        desc->field_4C = 0;
        desc->field_50 = 0;
        desc->field_51 = 0xFF;
        desc->field_52 = 0;
        desc->field_3C = 0;
        desc->field_40 = 0;
        desc->field_44 = 0;
        desc->field_38 = 0;
        desc->field_39 = 0;
    } while (0);
    return result;
}
