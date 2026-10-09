#include "common.h"

typedef unsigned char u8;
typedef float f32;
typedef union { u32 value; u8 channels[4]; } Color;
/* Same view as the caller func_80057974: packed color at +0, combiner bytes at +8. */
typedef struct { s32 palette; u8 unk4[4]; u8 color[16]; } Style;

/* Read-only color ramp (near, middle, far), owned by this file: gas fills the
 * jump delay slot with the %lo half of their addresses only for symbols defined
 * in the same file. */
const Color D_8014C110 = { 0xFFFEE100 };
const Color D_8014C114 = { 0xFEE88100 };
const Color D_8014C118 = { 0x9AA8AE00 };

extern float __builtin_sqrtf(float value);

void func_80057FC0(s32 x, s32 y, Style *style) {
    Color color;
    f32 distance;
    u32 packed;

    x -= 0xA0;
    y -= 0xDC;
    distance = __builtin_sqrtf((f32)(x * x + y * y));
    if (distance >= 200.0f) {
        color = D_8014C118;
    } else {
        const Color *start;
        const Color *end;
        f32 weight;
        s32 i;

        if (distance >= 60.0f) {
            start = &D_8014C114;
            end = &D_8014C118;
            weight = (distance - 60.0f) * (1.0f / 140.0f);
        } else {
            start = &D_8014C110;
            end = &D_8014C114;
            weight = distance * (1.0f / 60.0f);
        }
        for (i = 3; i != -1; i--) {
            color.channels[i] = (u32)((end->channels[i] - start->channels[i]) * weight + start->channels[i]);
        }
    }
    packed = color.value;
    style->color[0] = 3;
    style->color[1] = 31;
    style->color[2] = 1;
    style->color[3] = 31;
    style->color[4] = 7;
    style->color[5] = 7;
    style->color[6] = 7;
    style->color[7] = 1;
    style->color[8] = 3;
    style->color[9] = 31;
    style->color[10] = 1;
    style->color[11] = 31;
    style->color[12] = 7;
    style->color[13] = 7;
    style->color[14] = 7;
    style->color[15] = 1;
    style->palette = packed;
}
