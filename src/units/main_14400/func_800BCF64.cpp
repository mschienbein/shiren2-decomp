#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

struct Point {
    s32 x;
    s32 y;
};

struct Room {
    Point min;
    Point max;
    u8 pad10[4];
};

struct ShirenDirection {
    signed char value;
    ShirenDirection(s32 v) : value(v & 7) {}
};



/* Partial view of the floor generator object. */
struct Floor {
    u8 pad0[0x3DC];
    s32 room_count;
    u8 pad3E0[0x22];
    u16 field_402;
    u8 pad404[0x554];
    u16 flags;
    u8 pad95A[2];
    s32 room_open[16];
};

extern "C" {
extern Room D_801431F0[];
extern u8 D_80147620[];
u8 func_800C57CC(void *rng, s32 limit);
u8 func_800C57A0(void *rng);
s32 func_800C5844(void *rng, u8 base, u8 top);
s32 func_800A3138(Room *room);
s32 func_800A315C(Room *room);
s32 func_800BB22C(Floor *floor, Room *room);
s32 func_800BAFE4(Floor *floor, Room *room);
void func_800B1B58(Point *pos, u16 kind);
void *func_800A2594(Point *out, Point *from, ShirenDirection dir);
}



extern "C" s32 func_800BCF64(Floor *floor)
{
    s32 width = 0;
    s32 height = 0;
    s32 tries;

    if (!(floor->flags & 0x20)) {
        return 0;
    }
    Room *room = 0;
    for (tries = 100; --tries != -1;) {
        u8 index = func_800C57CC(D_80147620, (u8)(floor->room_count - 1));
        if (floor->room_open[index] == 0) {
            continue;
        }
        room = &D_801431F0[index];
        width = func_800A3138(room);
        height = func_800A315C(room);
        if (width < 5 || height < 5) {
            if (width < 5) {
                if (func_800BB22C(floor, room) != 0) {
                    width++;
                }
            }
            if (height < 5) {
                if (func_800BAFE4(floor, room) != 0) {
                    height++;
                }
            }
        }
        if (width >= 5 && height >= 5) {
            floor->room_open[index] = 0;
            break;
        }
    }
    if (tries < 0) {
        return 0;
    }
    Point center;
    s32 y = room->min.y + width / 2;
    center.y = y;
    if (!(width & 1)) {
        s32 odd = func_800C57A0(D_80147620) & 1;
        s32 adjusted = y;
        if (odd) adjusted--;
        center.y = adjusted;
    }
    s32 x = room->min.x + height / 2;
    center.x = x;
    if (!(height & 1)) {
        s32 odd = func_800C57A0(D_80147620) & 1;
        s32 adjusted = x;
        if (odd) adjusted--;
        center.x = adjusted;
    }
    func_800B1B58(&center, floor->field_402);
    s32 used[8];
    s32 i;
    for (i = 7; i >= 0; i--) {
        used[i] = 0;
    }
    u8 count = func_800C5844(D_80147620, 1, 8);
    while (--count != 0xFF) {
        u8 slot;
        for (;;) {
            u8 choice = func_800C57CC(D_80147620, 7);
            slot = choice;
            if (used[slot] == 0) {
                used[slot] = 1;
                break;
            }
        }
        Point pos;
        func_800A2594(&pos, &center, ShirenDirection(slot));
        func_800B1B58(&pos, floor->field_402);
    }
    return 1;
}
