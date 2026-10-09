#include "common.h"

typedef struct { unsigned char fields_00[0x46]; unsigned char field_46; unsigned char fields_47[0x29]; unsigned char field_70, field_71, field_72, field_73, field_74, field_75, field_76, field_77; } Effect;
typedef struct { s32 field_00; unsigned short field_04; unsigned char fields_06[2]; unsigned short field_08; unsigned char fields_0A[10]; s32 field_14, field_18, field_1C; } State;
extern s32 func_800751B4(s32, s32);
extern Effect *func_8007946C(s32, s32);
void func_800891E0(State *state) {
    Effect *effect = func_8007946C(0, state->field_14);
    switch (state->field_08) {
    case 0:
        func_800751B4(state->field_14, 0x8000);
        effect->field_70 = 0xFF;
        effect->field_71 = 0x20; effect->field_72 = 0x20;
        effect->field_73 = 0x80;
        effect->field_74 = 0; effect->field_75 = 0; effect->field_76 = 0;
        effect->field_77 = 0x80; effect->field_46 = 0xFF;
        state->field_1C = 4; state->field_08++;
        break;
    case 1:
        if (state->field_1C-- == 0) { func_800751B4(state->field_14, 0); state->field_04 = 4; }
        break;
    }
}
