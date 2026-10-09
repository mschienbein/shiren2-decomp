#include "common.h"

typedef struct { unsigned char fields_00[0x74]; unsigned char field_74[0x1C]; s32 field_90; } State;
s32 func_8009E0A8(State *state, s32 index) { if (index < 0 || index >= state->field_90) return 0x80000000; return state->field_74[index]; }
