#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Vec2i;

s32 func_800A21B0(s32 y, s32 x);

u8 *func_800A2228(u8 *out, Vec2i *delta) {
    s32 dir = func_800A21B0(delta->y, delta->x) * 2;
    s32 adjust = 0;

    if (dir & 2) {
        s32 absY = delta->y;
        s32 absX;

        if (absY < 0) {
            absY = -absY;
        }
        absX = delta->x;
        if (absX < 0) {
            absX = -absX;
        }
        if (absY != absX) {
            adjust = 1;
            if (absX < absY) {
                adjust = -1;
            }
            if (dir & 4) {
                adjust = -adjust;
            }
        }
    }
    *out = (dir + adjust) & 0xF;
    return out;
}
