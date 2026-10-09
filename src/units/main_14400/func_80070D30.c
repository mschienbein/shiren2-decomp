#include "common.h"

typedef unsigned char u8;
typedef struct { s32 words[4][4]; } Mtx;
typedef struct {
    u8 kind, flags, pad2[2];
    u32 field4, draw8, modeC, mode10, color14, env18;
    u8 combine1C[16];
    Mtx *matrix2C;
    void *field30;
    u8 pad34[4];
    u8 field38, field39, pad3A[2];
    u32 field3C, field40, field44, tile48;
    void *field4C;
    u8 red50, green51, blue52, pad53;
} Model;
extern float D_8013D4F0[];
extern const double D_8014C880;
extern Mtx *func_80070660(u32 count);
extern void func_80030FD0(Mtx *, float, float, float, float);
extern void func_8002CCC0(Mtx *, Mtx *, Mtx *);
extern void func_8002CFB0(Mtx *, float, float, float, float *, float *, float *);
extern float func_80027C60(float);
extern float func_80032360(float);
extern void func_80033A20(Mtx *, float, float, float);
extern void func_80031220(Mtx *, float, float, float);

/* Both callers supply alpha=255 and flags=0; this effect does not use them. */
s32 func_80070D30(Model *model, Mtx *parent, float height, s32 frame, s32 alpha, s32 index, s32 flags) {
    Mtx rotation;
    float x, y, z;
    s32 result = 0;
    Mtx *matrix;
    do { /* ODD_C: single-pass block grouping the matrix allocation and the whole
          * model build so allocation failure exits through break; this
          * early-exit form also shapes scheduling: the stack-passed index is
          * loaded into a saved register at entry, as in the original. */
        double radians;
        float translatedX;
        s32 phase;
        matrix = func_80070660(1);
        if (matrix == 0) {
            result = -1;
            break;
        }
        func_80030FD0(matrix, D_8013D4F0[index], 0.0f, 1.0f, 0.0f);
        func_80030FD0(&rotation, -90.0f, 1.0f, 0.0f, 0.0f);
        func_8002CCC0(&rotation, matrix, &rotation);
        func_8002CFB0(parent, 0.0f, 0.0f, 0.0f, &x, &y, &z);
        radians = D_8014C880;
        translatedX = x + func_80027C60((float)((double)D_8013D4F0[index] * radians)) * 18.0f;
        func_80033A20(matrix, translatedX, height + 14.0f,
            z - func_80032360((float)((double)D_8013D4F0[index] * radians)) * 18.0f);
        func_8002CCC0(&rotation, matrix, matrix);
        func_80031220(&rotation, 0.7f, 0.7f, 0.7f);
        func_8002CCC0(&rotation, matrix, matrix);
        model->kind = 1;
        model->flags = 9;
        model->field4 = 0x100000;
        model->matrix2C = matrix;
        model->field30 = 0;
        model->draw8 = 0x200005;
        model->modeC = 0x0C080000;
        model->mode10 = 0x104240;
        phase = (frame * 16) % 512;
        if (phase < 256) {
            model->color14 = ((phase & 255) << 16) | 0xFF0000FF;
        } else {
            model->color14 = ((~phase & 255) << 16) | 0xFF0000FF;
        }
        model->env18 = 255;
        model->combine1C[0] = 3;
        model->combine1C[1] = 5;
        model->combine1C[2] = 1;
        model->combine1C[3] = 5;
        model->combine1C[4] = 7;
        model->combine1C[5] = 7;
        model->combine1C[6] = 7;
        model->combine1C[7] = 1;
        model->combine1C[8] = 31;
        model->combine1C[9] = 31;
        model->combine1C[10] = 31;
        model->combine1C[11] = 0;
        model->combine1C[12] = 7;
        model->combine1C[13] = 7;
        model->combine1C[14] = 7;
        model->combine1C[15] = 0;
        model->tile48 = 0x171;
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
