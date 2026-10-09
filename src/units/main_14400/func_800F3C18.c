#include "common.h"
typedef struct { unsigned char fields00[0x7C]; unsigned short field7C; } State;
s32 func_800F3C18(State *state) {
    return (state->field7C >> 11) & 1;
}
