#include "common.h"

typedef struct { unsigned char fields_00[0x28]; unsigned short field_28; } State;
extern unsigned short func_800E08F0(State *);
unsigned short func_800E08B0(State *state) { unsigned short value = state->field_28; unsigned short limit = func_800E08F0(state); if (limit < value) value = limit; return value; }
