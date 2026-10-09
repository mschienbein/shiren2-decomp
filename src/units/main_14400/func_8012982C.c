#include "common.h"
typedef struct { unsigned char pad00[0xD2]; unsigned char fieldD2; } CommandState;
/* The dispatch table returns the next bytecode cursor. */
unsigned char *func_8012982C(CommandState *state, unsigned char *cursor) {
    state->fieldD2 = 0;
    return cursor;
}
