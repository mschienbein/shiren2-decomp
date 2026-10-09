#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef float f32;

/* Sequencer track state (pitch part). */
typedef struct {
    u8 pad00[0x24];
    f32 detune_24;
    f32 lastPitch_28;
    f32 targetPitch_2C;
    u8 pad30[0x4C - 0x30];
    f32 glideStart_4C;
    f32 pitch_50;
    u8 pad54[0xAA - 0x54];
    u16 elapsed_AA;
    u8 padAC[0xB8 - 0xAC];
    u8 glideTicks_B8;
    u8 padB9[0xBB - 0xB9];
    u8 volume_BB;
} Track8012B52C;

/* One output slot of the D_801CA6D8 table (0x1C bytes). */
typedef struct {
    u8 data[0x1C];
} Slot8012B52C;

extern Slot8012B52C *D_801CA6D8;
extern f32 func_8012BBF4(f32 x);
extern void func_80130150(Slot8012B52C *slot, f32 value);

/* Update the glide and send the pitch ratio 2^(semitones / 12), clamped to 2.0, to output slot `index`. */
void func_8012B52C(Track8012B52C *track, s32 index, f32 bend) {
    f32 pitch = track->targetPitch_2C;

    if (track->glideTicks_B8 != 0) {
        if (track->glideTicks_B8 >= track->elapsed_AA) {
            pitch = track->glideStart_4C +
                    (pitch - track->glideStart_4C) / (f32)track->glideTicks_B8 * (f32)track->elapsed_AA;
        }
        track->pitch_50 = pitch;
    }
    pitch += bend + track->detune_24;
    if (pitch != track->lastPitch_28) {
        track->lastPitch_28 = pitch;
        pitch = func_8012BBF4(pitch * (1.0 / 12));
        if (pitch > 2.0) {
            pitch = 2.0f;
            track->volume_BB = 0;
        }
        func_80130150(&D_801CA6D8[index], pitch);
    }
}
