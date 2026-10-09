#include "common.h"
typedef struct { unsigned char fields00[0x29]; unsigned char field29; } State;
s32 func_800F4CDC(State *state) {
    return state->field29 == 0;
}
