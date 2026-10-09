#include "common.h"

typedef struct { unsigned char fields_00[0x24]; void *field_24; } State;
extern unsigned char D_8015BE38[];
extern void func_800EFD28(State *, s32);
extern void func_800A3918(State *);
void func_80105F98(State *state, s32 flags) { state->field_24 = D_8015BE38; func_800EFD28(state, 0); if (flags & 1) func_800A3918(state); }
