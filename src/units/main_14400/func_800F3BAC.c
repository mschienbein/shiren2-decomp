#include "common.h"

typedef struct { unsigned char fields_00[0x9A]; unsigned short field_9A; } State;
void func_800F3BAC(State *state) { state->field_9A |= 1; }
