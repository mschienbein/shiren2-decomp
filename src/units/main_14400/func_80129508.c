#include "common.h"

typedef unsigned char u8;
typedef float f32;

/* Sequencer track state: vibrato/LFO parameters. */
typedef struct {
    u8 pad00[0x20];
    f32 depth_20;
    u8 pad24[0xB6 - 0x24];
    u8 delay_B6;
    u8 padB7[0xD5 - 0xB7];
    u8 period_D5;
    u8 padD6[0xDC - 0xD6];
    f32 phaseStep_DC;
} Track80129508;

/* Command: LFO delay, period (ticks) and negative depth (operand / 50). */
u8 *func_80129508(Track80129508 *track, u8 *cursor) {
    track->delay_B6 = *cursor++;
    track->period_D5 = *cursor++;
    track->depth_20 = -(f32)*cursor / 50.0;
    track->phaseStep_DC = 6.2831852 / (f32)track->period_D5;
    return cursor + 1;
}
