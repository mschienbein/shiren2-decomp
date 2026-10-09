#include "common.h"

typedef unsigned char u8;
typedef float f32;

/* One 0x50-byte output channel. */
typedef struct {
    u8 state;          /* 0x00 */
    u8 enabled;        /* 0x01 */
    u8 pad02[2];
    s32 field_04;
    s32 field_08;
    s32 field_0C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    u8 modeA[8];       /* 0x1C */
    u8 modeB[8];       /* 0x24 */
    f32 x;             /* 0x2C */
    f32 y;             /* 0x30 */
    f32 scaleX;        /* 0x34 */
    f32 scaleY;        /* 0x38 */
    u8 field_3C;
    u8 pad3D[3];
    s32 field_40;
    s32 field_44;
    s32 field_48;
    u8 field_4C;
    u8 field_4D;
    u8 field_4E;
    u8 pad4F;
} Output;

extern Output D_801DEA0C[2];

s32 func_8005CD40(s32 index, s32 arg1, s32 x, s32 y, s32 arg4, s32 arg5)
{
    Output *out = &D_801DEA0C[index];

    out->state = 2;
    out->field_3C = 0;
    out->enabled = 1;
    out->field_08 = 4;
    out->field_0C = 0x404240;
    out->field_10 = 0x104240;
    out->field_14 = -1;
    out->field_18 = -1;
    out->field_4D = 0xFE;
    out->x = x;
    out->y = y;
    out->field_4C = 1;
    out->field_4E = 0;
    out->field_04 = 0;
    out->modeA[0] = 0x1F;
    out->modeA[1] = 0x1F;
    out->modeA[2] = 0x1F;
    out->modeA[3] = 1;
    out->modeA[4] = 1;
    out->modeA[5] = 7;
    out->modeA[6] = 3;
    out->modeA[7] = 7;
    out->modeB[0] = 0x1F;
    out->modeB[1] = 0x1F;
    out->modeB[2] = 0x1F;
    out->modeB[3] = 1;
    out->modeB[4] = 1;
    out->modeB[5] = 7;
    out->scaleX = 1.0f;
    out->scaleY = 1.0f;
    out->field_40 = arg4;
    out->modeB[6] = 3;
    out->modeB[7] = 7;
    out->field_44 = arg1;
    out->field_48 = arg5;
    return 0;
}
