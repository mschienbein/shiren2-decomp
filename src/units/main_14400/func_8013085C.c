#include "common.h"

typedef float f32;

typedef struct {
    unsigned char pad0[0x40];
    s32 outputRate;
} Player;

extern Player *D_80148D84;

/* Convert a duration in microseconds into a sample count at the player's output rate. */
s32 func_8013085C(s32 usec)
{
    f32 samples = (f32)usec * (f32)D_80148D84->outputRate / 1000000.0 + 0.5;

    return samples;
}
