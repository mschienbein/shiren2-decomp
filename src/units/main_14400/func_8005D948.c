#include "common.h"

typedef unsigned short u16;
typedef unsigned char u8;
typedef struct {
    s32 field_00;
    s32 field_04;
    u16 field_08;
    u16 field_0A;
    u8 field_0C;
    u8 field_0D;
} State;
extern State D_80165960[];
void func_8005D948(s32 mode, s32 dx, s32 dy) {
    State *state = D_80165960;
    state->field_0D = mode;
    state->field_0C = 0;
    state->field_08 += dx;
    state->field_0A += dy;
}
