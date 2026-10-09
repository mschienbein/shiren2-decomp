#include "common.h"

typedef struct { unsigned char fields_00[0x20]; u32 field_20; } State;
void func_800A8078(State *state, u32 mask) { state->field_20 &= ~mask; }
