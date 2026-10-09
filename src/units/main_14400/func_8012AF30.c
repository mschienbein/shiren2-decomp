#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef float f32;

/* Envelope record consumed by func_80129240 (rate, level/length pairs). */
typedef struct {
    u8 data[7];
} Envelope;

typedef struct {
    u8 pad00[0x18];
    Envelope *envelopes;    /* 0x18 */
    u8 pad1C[4];
    u16 *instruments;       /* 0x20: program -> sample index, 0xFFFF = none */
} Bank;

typedef struct {
    u8 pad00[0x14];
    u16 *instruments;       /* 0x14 */
} AltBank;

typedef struct {
    u8 pad00[0x28];
    f32 *pitches;           /* 0x28: base pitch per sample */
    void **waves;           /* 0x2C: wave per sample */
} SampleSet;

/* Drum-map entry selected by note. */
typedef struct {
    u16 instrument;
    u16 envelope;
    u8 tuning;
    u8 note;
} KeyMap;

typedef struct {
    u8 data[0x1C];
} Slot;

typedef struct {
    u8 pad00[4];
    u8 *cursor;             /* 0x04 */
    void *pending;          /* 0x08 */
    s32 releaseTime;        /* 0x0C */
    u8 pad10[0x1C];
    f32 pitch;              /* 0x2C */
    u8 pad30[0xC];
    s32 clock;              /* 0x3C */
    s32 noteTime;           /* 0x40 */
    u8 pad44[8];
    f32 gain;               /* 0x4C */
    f32 nextGain;           /* 0x50 */
    s32 release;            /* 0x54 */
    u8 pad58[0x1C];
    Bank *bank;             /* 0x74 */
    AltBank *altBank;       /* 0x78 */
    SampleSet *samples;     /* 0x7C */
    u8 pad80[4];
    KeyMap *keyMap;         /* 0x84 */
    u8 pad88[0x12];
    u16 ticks;              /* 0x9A */
    u8 pad9C[4];
    u16 stopPending;        /* 0xA0 */
    u8 padA2[8];
    u16 elapsed;            /* 0xAA */
    u16 defaultTicks;       /* 0xAC */
    u16 instrument;         /* 0xAE */
    u8 padB0[7];
    u8 readTicks;           /* 0xB7 */
    u8 padB8;
    u8 transpose;           /* 0xB9 */
    u8 noTranspose;         /* 0xBA */
    u8 velocity;            /* 0xBB */
    u8 padBC;
    u8 tuning;              /* 0xBD */
    u8 padBE[5];
    u8 stage;               /* 0xC3 */
    u8 releaseRate;         /* 0xC4 */
    u8 advance;             /* 0xC5 */
    u8 padC6[3];
    u8 active;              /* 0xC9 */
    u8 pan;                 /* 0xCA */
    u8 panBase;             /* 0xCB */
    u8 lastPan;             /* 0xCC */
    u8 currentReleaseRate;  /* 0xCD */
    u8 padCE;
    u8 nextDuration;        /* 0xCF */
    u8 duration;            /* 0xD0 */
    u8 tied;                /* 0xD1 */
    u8 readVelocity;        /* 0xD2 */
    u8 defaultVelocity;     /* 0xD3 */
    u8 needsUpdate;         /* 0xD4 */
    u8 padD5;
    u8 started;             /* 0xD6 */
    u8 locked;              /* 0xD7 */
} Track;

#define NOTE_REST 0x60

extern u8 *(*D_801487D0[])(void *state, u8 *cursor);
extern Slot *D_801CA6D8;
extern s32 D_801CA6E8;
u8 *func_80129240(Track *track, u8 *cursor);
void func_8012B640(Track *track);
void func_8012B89C(Track *track);
void func_8012B36C(Track *track, s32 index);
void func_801301E0(Slot *slot, s16 value, s32 arg);
void func_80130020(Slot *slot, u8 value);
void func_80130320(Slot *slot);

/* Run the track's commands up to the next note, then start that note on
 * voice slot `channel` (or stop the slot when the track has ended). */
void func_8012AF30(Track *track, s32 channel)
{
    u8 *cursor;
    s32 note;
    u8 value; /* current command byte, note-length lead byte, transpose */

    cursor = track->cursor;
    while (cursor != 0) {
        value = *cursor;
        if ((s8)value >= 0) {
            break;
        }
        cursor = D_801487D0[value & 0x7F](track, cursor + 1);
    }
    track->cursor = cursor;
    if (cursor != 0) {
        track->gain = track->nextGain;
        note = *track->cursor++;
        if (track->readVelocity) {
            u8 velocity = *track->cursor++;

            if ((s8)velocity < 0) {
                velocity &= 0x7F;
                track->readVelocity = 0;
                track->defaultVelocity = velocity;
            }
            if (note != NOTE_REST) {
                track->velocity = velocity;
            }
        } else {
            track->velocity = track->defaultVelocity;
        }

        if (track->defaultTicks != 0) {
            if (track->readTicks) {
                track->readTicks = 0;
                value = *track->cursor++;
                if ((s8)value >= 0) {
                    track->ticks = value;
                } else {
                    track->ticks = *track->cursor++ + ((value & 0x7F) << 8);
                }
            } else {
                track->ticks = track->defaultTicks;
            }
        } else {
            value = *track->cursor++;
            if ((s8)value >= 0) {
                track->ticks = value;
            } else {
                track->ticks = *track->cursor++ + ((value & 0x7F) << 8);
            }
        }

        track->elapsed = 0;
        track->tied = 0;
        track->noteTime = track->clock;
        track->clock += track->ticks << 8;
        track->duration = track->nextDuration;

        if (track->bank != 0 && track->keyMap == 0 &&
            track->bank->instruments[track->instrument] == 0xFFFF) {
            note = NOTE_REST;
        }

        if (note != NOTE_REST) {
            SampleSet *samples = track->samples;
            u16 sample;
            f32 pitch;

            if (track->keyMap != 0) {
                track->instrument = track->keyMap[note].instrument;
                track->tuning = track->keyMap[note].tuning >> 1;
                func_80129240(track, track->bank->envelopes[track->keyMap[note].envelope].data);
                note = track->keyMap[note].note;
            }
            if (!track->started) {
                func_8012B640(track);
            }
            if (track->needsUpdate) {
                func_8012B89C(track);
            }
            sample = track->instrument;
            if (track->bank != 0) {
                sample = track->bank->instruments[sample];
            } else {
                sample = track->altBank->instruments[sample];
            }
            if (!track->locked) {
                track->pending = samples->waves[sample];
                if (track->active && track->stopPending) {
                    track->stopPending = 0;
                    func_801301E0(&D_801CA6D8[channel], 0, D_801CA6E8);
                } else {
                    func_8012B36C(track, channel);
                }
            }
            value = track->transpose * (1 - track->noTranspose);
            pitch = (f32)note + samples->pitches[sample];
            track->pitch = pitch;
            /* add the transpose as a signed byte */
            if (value & 0x80) {
                track->pitch = pitch + (f32)(value - 0x100);
            } else {
                track->pitch = pitch + (f32)value;
            }
            if (track->pan != track->lastPan) {
                u8 base = track->panBase;
                s32 spread = (0x80 - base) * track->pan;

                track->lastPan = track->pan;
                func_80130020(&D_801CA6D8[channel], base + (spread >> 7));
            }
        } else if (track->stage < 4) {
            track->stage = 4;
            track->advance = 1;
            track->release = track->releaseTime;
            track->currentReleaseRate = track->releaseRate;
        }
    } else if (track->active) {
        track->active = 0;
        func_801301E0(&D_801CA6D8[channel], 0, D_801CA6E8);
        func_80130320(&D_801CA6D8[channel]);
    }
}
