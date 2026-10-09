#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Sequencer command state: script base pointers and two loop cursors. */
typedef struct {
    u8 pad00[0x34];
    u8 *cursor_34;
    u8 *cursor_38;
    u8 pad3C[0x44];
    u8 *base_80;
    u8 *base_84;
    u8 *base_88;
    u8 *base_8C;
    u8 pad90[0x12];
    s16 active_A2;
    s16 active_A4;
} CommandState;

/* Big-endian 16-bit operands: jump offset, then two loop-target offsets. */
u8 *func_801298C4(CommandState *state, u8 *cursor)
{
    s32 offset;
    s32 value;

    offset = *cursor++ << 8;
    offset += *cursor++;
    value = *cursor++ << 8;
    value += *cursor++;
    state->active_A2 = 1;
    state->cursor_38 = state->base_8C + value;
    value = *cursor++ << 8;
    value += *cursor++;
    state->active_A4 = 1;
    state->cursor_34 = state->base_88 + value;
    return state->base_80 + offset;
}
