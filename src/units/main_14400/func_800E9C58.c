#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { u32 experience; } Threshold;
/* Player slot 0xA4 targets func_800EE060, which narrows its level for func_80044C60. */
typedef struct { u8 pad0[0xA0]; s16 adjustA0; s16 padA2; Threshold *(*thresholdA4)(void *, u8); } Methods;
typedef struct { u8 pad0[0x24]; const Methods *methods24; } Actor;
s32 func_800E9C58(Actor *actor, u32 experience) {
    s32 lower = 1;
    s32 upper;
    if (experience >= actor->methods24->thresholdA4((u8 *)actor + actor->methods24->adjustA0, 99)->experience) return 99;
    upper = 99;
    for (;;) {
        s32 mid;
        if ((u8)upper - (u8)lower < 2) break;
        mid = ((u8)upper + (u8)lower) / 2;
        if (experience >= actor->methods24->thresholdA4((u8 *)actor + actor->methods24->adjustA0, mid)->experience) lower = mid;
        else upper = mid;
    }
    return (u8)lower;
}
