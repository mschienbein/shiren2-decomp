#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x2C]; float field_2C; u8 pad_30[0x20]; float field_50; u8 pad_54[0x64]; u8 field_B8; } State;
u8 *func_80129210(State *state, u8 *cursor) { if ((state->field_B8 = *cursor++)) state->field_50 = state->field_2C; return cursor; }
