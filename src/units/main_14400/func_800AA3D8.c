#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Random Random;
extern Random D_80147620;
extern u8 func_800C57CC(void *rng, s32 limit);
s32 func_800AA3D8(u8 *weights, u8 count) {
    u16 total = 0;
    s32 i, draw, selected;
    for (i = 0; i < count; i++) total += weights[i];
    if (total == 0) return 0;
    draw = func_800C57CC(&D_80147620, (u8)(total - 1));
    for (selected = 0; selected < count; selected++) {
        s32 weight = weights[selected];
        if (draw < weight) return selected;
        draw -= weight;
    }
    return 0;
}
