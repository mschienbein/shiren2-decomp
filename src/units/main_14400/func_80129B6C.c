#include "common.h"

extern s32 D_801CA700;
extern s32 func_8012CEC8(s32 index);

/* Command handler from D_801487D0: the dispatcher passes its state object (unused here)
 * and the byte cursor; the advanced cursor is returned. */
unsigned char *func_80129B6C(void *state, unsigned char *cursor) {
    unsigned char value = *cursor++;

    if (D_801CA700 == 1) {
        func_8012CEC8(value);
    }
    return cursor;
}
