#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x84]; void *field_84; } State;
/* Command handlers consume and return a bytecode cursor. */
u8 *func_801298B0(State *state, u8 *cursor) {
    state->field_84 = 0;
    return cursor;
}
