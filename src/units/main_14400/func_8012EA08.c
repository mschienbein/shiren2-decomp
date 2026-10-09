#include "common.h"

typedef short s16;
typedef unsigned short u16;

/* Advance a 16-bit position by a signed 16.16 velocity scaled by step / 8. */
s16 func_8012EA08(s16 initial, s32 step, s16 integral, u16 fractional)
{
    s32 delta;

    step >>= 3;
    if (step == 0) {
        return initial;
    }
    delta = fractional * step;
    delta >>= 16;
    delta += integral * step;
    return initial + delta;
}
