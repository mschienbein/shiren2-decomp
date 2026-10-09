#include "common.h"

typedef struct { s32 fields_00[2]; void *field_08; } State;
extern unsigned char D_80153AA0[];
extern void func_800AC68C(void *);
void func_801171F8(State *state, s32 flags) { state->field_08 = D_80153AA0; if (flags & 1) func_800AC68C(state); }
