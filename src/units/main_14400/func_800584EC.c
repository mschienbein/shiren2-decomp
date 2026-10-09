#include "common.h"

typedef struct { unsigned char field_00, field_01; unsigned char fields_02[2]; s32 field_04, field_08, field_0C, field_10; unsigned char fields_14[8]; unsigned char field_1C, field_1D, field_1E, field_1F, field_20, field_21, field_22, field_23, field_24, field_25, field_26, field_27, field_28, field_29, field_2A, field_2B; unsigned char fields_2C[8]; float field_34, field_38; unsigned char field_3C; unsigned char fields_3D[0x13]; } State;
extern unsigned char *func_8006A810(void *, s32, s32);
void func_800584EC(State *state) {
    func_8006A810(state, 0, 0x50);
    state->field_00 = 2;
    state->field_3C = 0;
    state->field_01 = 1;
    state->field_08 = 0x200004;
    state->field_04 = 0;
    state->field_0C = 0x443048;
    state->field_10 = 0x113048;
    state->field_1C = 31; state->field_1D = 31; state->field_1E = 31; state->field_1F = 1;
    state->field_20 = 7; state->field_21 = 7; state->field_22 = 7; state->field_23 = 1;
    state->field_24 = 31; state->field_25 = 31; state->field_26 = 31; state->field_27 = 1;
    state->field_28 = 7; state->field_29 = 7; state->field_2A = 7; state->field_2B = 1;
    state->field_34 = 1.0f; state->field_38 = 1.0f;
}
