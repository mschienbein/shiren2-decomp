#include "common.h"

typedef struct { s32 fields_00[2]; void *field_08; } State;
extern unsigned char D_80160568[];
extern State *func_801128F0(State *, s32);
State *func_801271B0(State *state) { func_801128F0(state, 0xEA); state->field_08 = D_80160568; return state; }
