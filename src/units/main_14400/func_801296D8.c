#include "common.h"

typedef unsigned char u8;
typedef struct CommandState {
    u8 pad_00[0xD7];
    u8 flag_D7;
} CommandState;

/* D_801487D0 dispatch entries consume and return a bytecode cursor. */
u8 *func_801296D8(CommandState *state, u8 *cursor)
{
    state->flag_D7 = 0;
    return cursor;
}
