#include "common.h"

typedef struct {
    s32 field_00;
    unsigned short status_04;
    unsigned short field_06;
    unsigned short state_08;
    unsigned short field_0A;
    s32 fields_0C[2];
    s32 id_14;
    s32 fields_18[3];
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 fields_30[6];
    float speed_48;
    float time_4C;
    float height_50;
    s32 fields_54[2];
    s32 field_5C;
    s32 fields_60[2];
    s32 field_68;
} Effect;

typedef struct {
    s32 fields_00[5];
    float height_14;
    s32 fields_18[9];
    unsigned short field_3C;
} Sprite;

extern unsigned char D_801C3395[];
extern s32 func_800748F8(s32, s32, s32, s32, s32, s32);
extern Sprite *func_8007946C(s32, s32);
extern s32 func_80076044(s32, s32, s32, s32, s32);
extern s32 func_800751B4(s32, s32);

void func_80087FEC(Effect *effect) {
    Sprite *sprite;
    switch (effect->state_08) {
    case 0:
        if (D_801C3395[effect->id_14] == 0) {
            D_801C3395[effect->id_14] = 1;
            func_800748F8(effect->id_14, effect->field_24, effect->field_5C,
                          effect->field_68, effect->field_2C, effect->field_28);
        }
        sprite = func_8007946C(0, effect->id_14);
        func_80076044(effect->id_14, sprite->field_3C, 2, 8, 3);
        func_800751B4(effect->id_14, 0x8000);
        sprite->height_14 = 200.0f;
        effect->speed_48 = 0.0f;
        effect->time_4C = 0.0f;
        effect->height_50 = sprite->height_14;
        ++effect->state_08;
        break;
    case 1:
        sprite = func_8007946C(0, effect->id_14);
        effect->speed_48 = 9.8f - effect->speed_48 * 0.1f;
        effect->time_4C += 0.1f;
        effect->height_50 -= effect->speed_48;
        if (effect->height_50 <= 0.0f) {
            effect->height_50 = 0.0f;
            ++effect->state_08;
        }
        sprite->height_14 = effect->height_50;
        break;
    case 2:
        sprite = func_8007946C(0, effect->id_14);
        func_80076044(effect->id_14, sprite->field_3C, 1, 8, 3);
        func_800751B4(effect->id_14, 0);
        effect->status_04 = 4;
        break;
    }
}
