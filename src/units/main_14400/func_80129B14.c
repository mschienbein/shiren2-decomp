#include "common.h"

typedef unsigned char u8;
typedef float f32;

/* Sequencer track state: field_24 caches field_70 * field_6C (see func_80129750). */
typedef struct {
    u8 pad00[0x24];
    f32 field_24;
    u8 pad28[0x6C - 0x28];
    f32 field_6C;
    f32 field_70;
} Track80129B14;

/* Command: set the scale to operand / 64 and refresh the scaled value. */
u8 *func_80129B14(Track80129B14 *track, u8 *cursor) {
    f32 scale = (f32)*cursor * 0.015625;

    track->field_6C = scale;
    track->field_24 = track->field_70 * scale;
    return cursor + 1;
}
