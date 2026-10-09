#include "common.h"

typedef unsigned char u8;
typedef struct DrawSlot {
    u8 mode_00, enabled_01;
    u8 pad_02[2];
    s32 field_04;
    u32 flags_08, flags_0C, flags_10;
    s32 field_14, field_18;
    u8 combine_1C[16];
    float x_2C, y_30, scale_x_34, scale_y_38;
    u8 field_3C;
    u8 pad_3D[3];
    s32 layer_40, image_44, palette_48;
    u8 enabled_4C, alpha_4D, field_4E, pad_4F;
} DrawSlot;
extern DrawSlot D_801DEA0C[2];

s32 func_8005CC34(s32 index, s32 image, s32 x, s32 y, s32 layer, s32 palette)
{
    DrawSlot *slot = &D_801DEA0C[index];
    slot->mode_00 = 2;
    slot->field_3C = 0;
    slot->enabled_01 = 1;
    slot->flags_08 = 0x200004;
    slot->flags_0C = 0x4041C8;
    slot->flags_10 = 0x1041C8;
    slot->field_14 = -1;
    slot->field_18 = -1;
    slot->alpha_4D = 0xFE;
    slot->x_2C = x;
    slot->y_30 = y;
    slot->enabled_4C = 1;
    slot->field_4E = 0;
    slot->field_04 = 0;
    slot->combine_1C[0] = 6;
    slot->combine_1C[1] = 1;
    slot->combine_1C[2] = 6;
    slot->combine_1C[3] = 31;
    slot->combine_1C[4] = 1;
    slot->combine_1C[5] = 7;
    slot->combine_1C[6] = 3;
    slot->combine_1C[7] = 7;
    slot->combine_1C[8] = 6;
    slot->combine_1C[9] = 1;
    slot->combine_1C[10] = 6;
    slot->combine_1C[11] = 31;
    slot->combine_1C[12] = 1;
    slot->combine_1C[13] = 7;
    slot->scale_x_34 = 1.0f;
    slot->scale_y_38 = 1.0f;
    slot->layer_40 = layer;
    slot->combine_1C[14] = 3;
    slot->combine_1C[15] = 7;
    slot->image_44 = image;
    slot->palette_48 = palette;
    return 0;
}
