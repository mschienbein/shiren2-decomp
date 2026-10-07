#include "common.h"

typedef struct {
    s32 field_0;
    s32 field_4;
    s32 field_8;
} State;

extern State D_80161724;

void func_80054968(void)
{
    State *state = &D_80161724;

    if (state->field_8 < 2) {
        state->field_8 = 1;
    }
}
