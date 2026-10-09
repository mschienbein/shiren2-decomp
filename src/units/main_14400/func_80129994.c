#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0xBC]; u8 fieldBC; } ScriptState;
extern s32 func_8012BF6C(s32 range);
u8 *func_80129994(ScriptState *state, u8 *cursor) {
    state->fieldBC = func_8012BF6C(*cursor++);
    state->fieldBC += *cursor++;
    return cursor;
}
