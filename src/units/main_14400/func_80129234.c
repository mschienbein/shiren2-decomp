#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0xB8]; u8 fieldB8; } ScriptState;
u8 *func_80129234(ScriptState *state, u8 *cursor) {
    state->fieldB8 = 0;
    return cursor;
}
