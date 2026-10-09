#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0xCE]; u8 field_CE; } State;
/* D_801487D0 dispatch consumes and returns a bytecode cursor. */
u8 *func_80129810(State *state, u8 *cursor) {
    state->field_CE = 0;
    return cursor;
}
