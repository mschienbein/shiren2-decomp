#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef float f32;

/* Sequencer track state: tempo clock and the pitch-sweep command stream. */
typedef struct {
    u8 pad00[0xC];
    s32 target_0C;
    u8 pad10[0x18 - 0x10];
    s32 clock_18;
    u8 pad1C[0x24 - 0x1C];
    f32 scaled_24;
    u8 pad28[0x34 - 0x28];
    u8 *cursor_34;
    u8 pad38[0x6C - 0x38];
    f32 scale_6C;
    f32 value_70;
    u8 pad74[0xA4 - 0x74];
    u16 wait_A4;
} Track8012BADC;

/*
 * Advance the clock in 0x100 steps until it reaches target_0C; whenever the wait
 * counter expires read the next value (centred on 64) and, when bit 7 of the value
 * byte is set, a one- or two-byte wait (high bit marks the two-byte form).
 */
void func_8012BADC(Track8012BADC *track) {
    u8 value;

    do {
        track->clock_18 += 0x100;
        if (--track->wait_A4 == 0) {
            value = *track->cursor_34++;
            if ((signed char)value < 0) {
                track->value_70 = (f32)(value & 0x7F) - 64.0;
                track->scaled_24 = track->value_70 * track->scale_6C;
                value = *track->cursor_34++;
                if ((signed char)value < 0) {
                    track->wait_A4 = (value & 0x7F) << 8;
                    track->wait_A4 += *track->cursor_34++ + 2;
                } else {
                    track->wait_A4 = value + 2;
                }
            } else {
                track->value_70 = (f32)value - 64.0;
                track->scaled_24 = track->value_70 * track->scale_6C;
                track->wait_A4 = 1;
            }
        }
    } while (track->clock_18 - track->target_0C < 0);
}
