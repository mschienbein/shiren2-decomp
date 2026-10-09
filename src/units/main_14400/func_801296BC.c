#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0xD6]; u8 fieldD6; } CommandState;

/* The shared command dispatcher consumes the returned byte-stream cursor. */
u8 *func_801296BC(CommandState *state, u8 *cursor)
{
    state->fieldD6 = 0;
    return cursor;
}
