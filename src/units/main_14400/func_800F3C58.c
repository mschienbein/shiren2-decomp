#include "common.h"

typedef struct {
    char reserved_00[0x7C];
    unsigned short field_7C;
} State;

s32 func_800F3C58(State *state) {
    return (state->field_7C >> 2) & 1;
}
