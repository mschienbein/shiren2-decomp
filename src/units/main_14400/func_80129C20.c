#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0xAC]; unsigned short field_AC; } State;
u8 *func_80129C20(State *state, u8 *cursor) { state->field_AC = 0; return cursor; }
