#include "common.h"

typedef unsigned char u8;

/*
 * Stream state (partial prefix view). The copy helpers func_800CA610/func_800CA668
 * form buffer + position from +0x1C/+0x20 and advance the position, so +0x1C is the
 * byte-buffer pointer, +0x20 the current byte position and +0x24 the byte capacity.
 */
typedef struct {
    u8 pad0[0x1C];
    u8 *buffer;
    u32 position;
    u32 capacity;
} StreamState800CA600;

void func_800CA600(StreamState800CA600 *state, u8 *buffer, u32 capacity)
{
    state->buffer = buffer;
    state->position = 0;
    state->capacity = capacity;
}
