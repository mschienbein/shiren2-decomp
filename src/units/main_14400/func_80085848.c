#include "common.h"
typedef struct { unsigned char fields00[7]; unsigned char field07; unsigned char field08; unsigned char field09; unsigned char fields0A[0x34]; unsigned char field3E; unsigned char field3F; unsigned char field40; } Target;
typedef struct { s32 field00; unsigned short field04; unsigned short field06; unsigned short field08; unsigned char fields0A[0xA]; s32 field14; s32 field18; s32 field1C; s32 field20; s32 field24; s32 field28; s32 field2C; } State;
extern Target *func_8007946C(s32 zero, s32 id);
void func_80085848(State *state) {
    Target *target = func_8007946C(0, state->field14);
    switch (state->field08) {
    case 0:
        state->field24 = target->field40;
        state->field28 = target->field3E;
        target->field07 = 2;
        target->field40 = 1;
        target->field3E = state->field18;
        state->field08++;
        return;
    case 1:
        if (state->field1C-- == 0) {
            target = func_8007946C(0, state->field14);
            if (state->field2C == 0) {
                target->field40 = state->field24;
                target->field3E = state->field28;
            } else {
                target->field09 = 0;
            }
            target->field07 = 1;
            state->field08++;
        }
        return;
    case 2:
        state->field04 = 4;
        break;
    }
}
