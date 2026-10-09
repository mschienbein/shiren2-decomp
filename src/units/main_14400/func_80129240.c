#include "common.h"

typedef unsigned char u8;
typedef float f32;

/* Sequencer track state: three-segment envelope (levels C0..C2, segment lengths C6..C8). */
typedef struct {
    u8 pad00[0x58];
    f32 slope1_58;
    f32 slope2_5C;
    f32 releaseStep_60;
    s32 rateStep_64;
    u8 pad68[0xBF - 0x68];
    u8 rate_BF;
    u8 level0_C0;
    u8 level1_C1;
    u8 level2_C2;
    u8 padC3[0xC6 - 0xC3];
    u8 length1_C6;
    u8 length2_C7;
    u8 length3_C8;
} Track80129240;

/* Command: rate, then (level, length) pairs; precompute per-tick envelope slopes. */
u8 *func_80129240(Track80129240 *track, u8 *cursor)
{
    u8 value = *cursor++;

    if (value == 0) {
        value = 1;
    }
    track->rate_BF = value;
    track->rateStep_64 = 0x400 / value;
    track->level0_C0 = *cursor++;
    value = *cursor++;
    track->length1_C6 = value;
    track->level1_C1 = *cursor++;
    track->slope1_58 = (1.0 / (f32)value) * (f32)(track->level1_C1 - track->level0_C0);
    value = *cursor++;
    track->length2_C7 = value;
    track->level2_C2 = *cursor++;
    track->slope2_5C = (1.0 / (f32)value) * (f32)(track->level2_C2 - track->level1_C1);
    value = *cursor++;
    track->length3_C8 = value;
    track->releaseStep_60 = 1.0 / (f32)value;
    return cursor;
}
