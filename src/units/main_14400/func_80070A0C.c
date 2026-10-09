#include "common.h"

typedef unsigned char u8;
typedef struct { s32 words[4][4]; } Mtx;
typedef union { u32 word; u8 rgba[4]; } Color;
typedef struct {
    u8 kind, flags, pad2[2];
    u32 field4, draw8, modeC, mode10;
    Color color14;
    u32 env18;
    u8 combine1C[16];
    Mtx *matrix2C;
    void *field30;
    u8 pad34[4];
    u8 field38, field39, pad3A[2];
    u32 field3C, field40, field44, tile48;
    void *field4C;
    u8 red50, green51, blue52, pad53;
} Model;
extern const double D_8014C870, D_8014C878;
extern Mtx *func_80070660(u32 count);
extern void func_80030FD0(Mtx *, float, float, float, float);
extern void func_8002CCC0(Mtx *, Mtx *, Mtx *);
extern void func_8002CFB0(Mtx *, float, float, float, float *, float *, float *);
extern float func_80032360(float);
extern void func_80033A20(Mtx *, float, float, float);
extern void func_80031220(Mtx *, float, float, float);

s32 func_80070A0C(Model *model, Mtx *parent, float height, s32 frame, s32 alpha) {
    Mtx rotation;
    float x, y, z;
    s32 result = 0;
    Mtx *matrix;
    /* ODD_C: group fallible matrix construction with its early exit; this also shapes scheduling. */
    do {
        float phase;
        float scaleX;
        s32 opacity;
        matrix = func_80070660(1);
        if (matrix == 0) {
            result = -1;
            break;
        }
        func_80030FD0(&rotation, -90.0f, 1.0f, 0.0f, 0.0f);
        func_8002CFB0(parent, 0.0f, 0.0f, 0.0f, &x, &y, &z);
        func_80033A20(matrix, x, height, z);
        func_8002CCC0(&rotation, matrix, matrix);
        phase = (float)((double)((float)((frame * 8) % 128) * 0.00390625f) * D_8014C870);
        scaleX = (float)((double)func_80032360(phase) * 0.5 + D_8014C878);
        func_80031220(&rotation, scaleX, (float)((double)func_80032360(phase) * 0.5 + D_8014C878), 1.0f);
        func_8002CCC0(&rotation, matrix, matrix);
        if (alpha == 255) opacity = (s32)(255.0f - func_80032360(phase) * 255.0f);
        else opacity = 0;
        model->kind = 1;
        /* FAKEMATCH: the ROM writes the byte channels before the packed colour overwrite below;
           dropping these dead stores (and the env18 reset) loses 20 original code bytes. */
        model->color14.rgba[0] = 0;
        model->draw8 = 0x200005;
        model->field4 = 0x100000;
        model->flags = 1;
        model->color14.rgba[1] = 0;
        model->color14.rgba[2] = 0;
        model->color14.rgba[3] = opacity;
        model->modeC = 0x0C080000;
        model->matrix2C = matrix;
        model->color14.word = 0xFFFFFF00;
        model->combine1C[0] = 3;
        model->combine1C[1] = 5;
        model->field30 = 0;
        model->mode10 = 0x104B50;
        model->combine1C[2] = 1;
        /* FAKEMATCH: overwritten environment reset retained with the byte-channel writes. */
        model->env18 = 0;
        model->combine1C[3] = 5;
        model->combine1C[4] = 1;
        model->combine1C[6] = 5;
        model->combine1C[5] = 7;
        model->env18 = ((opacity * alpha / 255) & 255) | 0x73A0F100;
        model->combine1C[8] = 31;
        model->combine1C[9] = 31;
        model->combine1C[7] = 7;
        model->combine1C[11] = 0;
        model->combine1C[12] = 7;
        model->combine1C[10] = 31;
        model->combine1C[13] = 7;
        model->combine1C[14] = 7;
        model->combine1C[15] = 0;
        model->tile48 = 0x13A;
        model->field4C = 0;
        model->red50 = 0;
        model->green51 = 255;
        model->blue52 = 0;
        model->field3C = 0;
        model->field40 = 0;
        model->field44 = 0;
        model->field38 = 0;
        model->field39 = 0;
    } while (0);
    return result;
}
