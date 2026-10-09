#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_0[4]; u16 field_4; u16 pad_6; u16 field_8; u8 pad_A[10]; s32 field_14; s32 field_18; s32 field_1C; } State;
typedef struct Unit8004D170 Unit8004D170;
extern Unit8004D170 *func_8007946C(s32 side, s32 slot);
extern s32 func_80076044(s32, s32, s32, s32, s32);
extern void func_80079560(s32 a, s32 b, s32 mode);
void func_800879D4(State *state)
{
    func_8007946C(0, state->field_14);
    switch (state->field_8) {
    case 0:
        state->field_1C = 0;
        state->field_8++;
        /* fall through */
    case 1:
        if (state->field_1C-- == 0) {
            func_80076044(state->field_14, 0, 1, 8, 3);
            func_80079560(0, state->field_14, 0);
            state->field_4 = 4;
        }
        break;
    }
}
