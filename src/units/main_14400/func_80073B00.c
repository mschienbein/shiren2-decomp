#include "common.h"
typedef unsigned char u8;
typedef struct {
    u8 mode_00, enabled_01; u8 pad_02[2]; u32 field_04, draw_08, render_mode_0C, render_mode_10, color_14;
    u32 field_18; u8 components_1C[16]; float x_2C, y_30, scale_x_34, scale_y_38;
    u8 flags_3C; u8 pad_3D[3]; s32 duration_40, tile_44, layer_48; u8 red_4C, green_4D, blue_4E, pad_4F;
} Sprite;
typedef struct { s32 active_00; Sprite sprite_04; float velocity_x_54, velocity_y_58; s32 phase_5C; float timer_60; } Particle;
extern Particle *D_801A7224;
u32 func_8006A7A8(unsigned short maximum);
s32 func_80073B00(float x)
{
    Particle *particle = D_801A7224;
    s32 result = -1;
    s32 i;
    for (i = 0; i < 28; i++, particle++) {
        if (particle->active_00 < 0) {
            Sprite *sprite = &particle->sprite_04;
            sprite->x_2C = x - (float)func_8006A7A8(8);
            sprite->y_30 = (float)(200 - func_8006A7A8(8));
            sprite->duration_40 = 100;
            sprite->scale_x_34 = 0.75f;
            sprite->scale_y_38 = 0.75f;
            sprite->flags_3C = 0;
            particle->sprite_04.mode_00 = 2;
            sprite->tile_44 = 0xF7;
            sprite->layer_48 = 12;
            sprite->enabled_01 = 1;
            sprite->draw_08 = 0x200004;
            sprite->field_04 = 0;
            sprite->render_mode_0C = 0x4041C8;
            sprite->render_mode_10 = 0x1041C8;
            sprite->components_1C[0] = 1;
            sprite->components_1C[1] = 31;
            sprite->components_1C[2] = 3;
            sprite->components_1C[3] = 31;
            sprite->components_1C[4] = 1;
            sprite->components_1C[5] = 7;
            sprite->components_1C[6] = 3;
            sprite->components_1C[7] = 7;
            sprite->components_1C[8] = 1;
            sprite->components_1C[9] = 31;
            sprite->components_1C[10] = 3;
            sprite->components_1C[11] = 31;
            sprite->components_1C[12] = 1;
            sprite->components_1C[13] = 7;
            sprite->components_1C[14] = 3;
            sprite->components_1C[15] = 7;
            sprite->color_14 = 0xFFFAF080;
            sprite->red_4C = 0;
            sprite->green_4D = 0xFE;
            sprite->blue_4E = 0;
            particle->active_00 = 0;
            particle->velocity_x_54 = 0.0f - (float)func_8006A7A8(32) * 0.03125f;
            particle->velocity_y_58 = 0.0f - (float)func_8006A7A8(32) * 0.03125f;
            particle->phase_5C = 0;
            particle->timer_60 = 0.0f;
            result = 0;
            break;
        }
    }
    return result;
}
