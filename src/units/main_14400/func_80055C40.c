#include "common.h"

typedef struct {
    char reserved_00[0x17];
    unsigned char field_17;
    char reserved_18[0x38];
    short field_50;
    short field_52;
    char reserved_54[6];
    short field_5A;
} State;

void func_80055C40(State *state) {
    short elapsed = state->field_5A + 1;
    s32 value;
    state->field_5A = elapsed;
    value = (s32)(((float)elapsed / (float)state->field_52) * 255.0f);
    state->field_17 = value;
    if ((u32)(value & 0xFF) >= 0xFFU) {
        state->field_50 = 2;
        state->field_5A = 0;
    }
}
