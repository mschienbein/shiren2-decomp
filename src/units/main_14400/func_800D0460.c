#include "common.h"

typedef struct { void *field_00, *field_04; } Pair;
typedef struct { unsigned char pad_00[8]; Pair *field_08; s32 field_0C, field_10; } State;
extern s32 func_800CD2BC(void *, void *);
void func_800D0460(State *state, s32 index) { Pair *entry = state->field_08 + index; func_800CD2BC(entry->field_00, entry->field_04); for (; index < state->field_10 - 1; index++) state->field_08[index] = state->field_08[index + 1]; state->field_10--; }
