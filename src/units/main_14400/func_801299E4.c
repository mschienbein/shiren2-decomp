#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0xBD]; u8 fieldBD; } CommandState;
extern s32 func_8012BF6C(s32 range);

u8 *func_801299E4(CommandState *state, u8 *cursor)
{
    s32 value = func_8012BF6C(*cursor++);
    state->fieldBD = value;
    value += *cursor++;
    state->fieldBD = value;
    return cursor;
}
