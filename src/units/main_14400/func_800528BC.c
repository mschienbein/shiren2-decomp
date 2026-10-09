#include "common.h"

typedef unsigned char u8;
/* Pan/volume byte pair (0x80 = centred); same storage as func_80052260's view. */
typedef struct { u8 x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state: type +0, playback.handle +4, playback.position +8/+9. */
typedef struct { short type; Playback playback; } State;

extern State D_80161644, D_80161650;
extern Pair D_8014BF98;

/* Resets both sound states, then restores the music state's default position. */
void func_800528BC(void) {
    Playback *music, *sound;
    State *state;

    music = &D_80161644.playback;
    music->handle = 0;
    state = &D_80161644;
    state->type = 0;
    music->position.x = 0;
    music->position.y = 0x80;

    sound = &D_80161650.playback;
    sound->handle = 0;
    state = &D_80161650;
    state->type = 0;
    sound->position.x = 0;
    sound->position.y = 0x80;

    music = &D_80161644.playback;
    music->handle = 0;
    state = &D_80161644;
    state->type = 0;
    music->position = D_8014BF98;
}
