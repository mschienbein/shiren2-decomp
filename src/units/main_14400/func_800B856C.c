#include "common.h"

typedef struct { s32 x, y; } Position;

extern void func_800B83E8(Position *center, s32 first, s32 second);

/* Midpoint circle walk over one octant; func_800B83E8 plots the mirrored spans. */
void func_800B856C(Position *center, s32 radius)
{
    s32 x = 0;
    s32 y = radius - 1;
    s32 error = radius;

    while (x <= y) {
        func_800B83E8(center, x, y);
        x++;
        {
            s32 next = error + 1;
            error = next - 2 * x;
        }
        if (error <= 0) {
            s32 next;
            y--;
            next = error - 1;
            error = next + 2 * y;
        }
    }
}
