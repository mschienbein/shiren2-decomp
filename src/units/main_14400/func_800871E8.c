#include "common.h"

typedef struct {
    s32 field_00;
    unsigned short status_04;
    unsigned short field_06;
    unsigned short state_08;
    unsigned short field_0A;
    s32 fields_0C[2];
    s32 id_14;
    s32 field_18;
    s32 remaining_1C;
    s32 fields_20[5];
    float speed_34;
} Effect;

typedef struct {
    s32 fields_00[5];
    float height_14;
} Sprite;

extern Sprite *func_8007946C(s32, s32);
extern void func_80079560(s32, s32, s32);
extern void func_8007935C(s32);

void func_800871E8(Effect *effect) {
    Sprite *sprite = func_8007946C(4, effect->id_14);
    switch (effect->state_08) {
    case 0:
        func_80079560(4, effect->id_14, 0);
        effect->remaining_1C = 10;
        effect->speed_34 = 20.0f;
        ++effect->state_08;
        /* Fall through into the first animation step. */
    case 1:
        if (effect->remaining_1C-- != 0) {
            sprite->height_14 += effect->speed_34;
            effect->speed_34 *= 1.35f;
        } else {
            func_8007935C(effect->id_14);
            effect->status_04 = 4;
        }
        break;
    }
}
