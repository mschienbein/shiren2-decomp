#include "common.h"

typedef union { u32 word; unsigned char bytes[4]; } Color;
typedef struct { unsigned char fields_00[0x14]; Color field_14; unsigned char fields_18[0x38]; short field_50, field_52, field_54, field_56, field_58, field_5A; } State;
static inline unsigned char fade(s32 amount) { return 255 - (u32)(((float)(255 - amount) / 255.0f) * 255.0f); }
void func_8005690C(State *state) {
    unsigned short next = state->field_54;
    if (state->field_54 != -1) {
        s32 amount;
        if (state->field_54 >= state->field_5A + 1) next = state->field_5A + 1;
        state->field_5A = next;
        amount = 255 - (s32)(((float)(short)next / (float)state->field_54) * 255.0f);
        state->field_14.bytes[0] = fade(amount);
        state->field_14.bytes[1] = fade(amount);
        state->field_14.bytes[2] = fade(amount);
        if (!(state->field_14.word & ~0xFF)) { state->field_50 = 3; state->field_5A = 0; }
    }
}
