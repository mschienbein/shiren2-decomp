#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x, y; } Vec2;

typedef struct Rng800BF2F4 Rng800BF2F4;
typedef struct Map800BF2F4 Map800BF2F4;

extern Rng800BF2F4 D_80147620;

extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern void func_800BEFB0(void *map, Vec2 *from, Vec2 *to, s32 started);

/* Pick one exit on each of two edge groups and dig corridors from them to the room center. */
void func_800BF2F4(Map800BF2F4 *map) {
    Vec2 center;
    s32 exits[8];
    Vec2 edge;
    s32 i;

    center.y = 0x26;
    center.x = 0x1B;
    for (i = 7; i >= 0; i--) {
        exits[i] = 0;
    }
    exits[(u8)func_800C5844(&D_80147620, 0, 3)] = 1;
    exits[(u8)func_800C5844(&D_80147620, 4, 7)] = 1;
    if (exits[0]) {
        edge.y = (u8)func_800C5844(&D_80147620, center.y + 3, 0x41);
        edge.x = 0xA;
        func_800BEFB0(map, &edge, &center, 0);
    }
    if (exits[2]) {
        edge.y = (u8)func_800C5844(&D_80147620, center.y + 3, 0x41);
        edge.x = 0x2B;
        func_800BEFB0(map, &edge, &center, 0);
    }
    if (exits[4]) {
        edge.y = (u8)func_800C5844(&D_80147620, 0xA, center.y - 3);
        edge.x = 0x2B;
        func_800BEFB0(map, &edge, &center, 0);
    }
    if (exits[6]) {
        edge.y = (u8)func_800C5844(&D_80147620, 0xA, center.y - 3);
        edge.x = 0xA;
        func_800BEFB0(map, &edge, &center, 0);
    }
    if (exits[1]) {
        edge.y = 0x41;
        edge.x = (u8)func_800C5844(&D_80147620, 0xA, center.x - 3);
        func_800BEFB0(map, &edge, &center, 1);
    }
    if (exits[3]) {
        edge.y = 0x41;
        edge.x = (u8)func_800C5844(&D_80147620, center.x + 3, 0x2B);
        func_800BEFB0(map, &edge, &center, 1);
    }
    if (exits[5]) {
        edge.y = 0xA;
        edge.x = (u8)func_800C5844(&D_80147620, center.x + 3, 0x2B);
        func_800BEFB0(map, &edge, &center, 1);
    }
    if (exits[7]) {
        edge.y = 0xA;
        edge.x = (u8)func_800C5844(&D_80147620, 0xA, center.x - 3);
        func_800BEFB0(map, &edge, &center, 1);
    }
}
