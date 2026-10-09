#include "common.h"

typedef struct { unsigned short field_00; const void *field_04; } State;
extern unsigned char D_80157FA8[];
extern const unsigned char D_80158A18[48];
State *func_800DEEC0(State *state) { state->field_04 = D_80157FA8; state->field_00 = 0x2D; state->field_04 = D_80158A18; return state; }
