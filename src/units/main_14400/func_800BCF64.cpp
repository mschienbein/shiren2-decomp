#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;
typedef unsigned short u16;

struct Room_800BCF64 {
    MapRect geometry;
    u8 sides[4];
};

struct ShirenDirection_800BCF64 {
    signed char value;
    ShirenDirection_800BCF64(s32 v) : value(v & 7) {}
};



/* Partial view of the floor generator object. */
struct Floor_800BCF64 {
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
extern Room_800BCF64 D_801431F0[];
extern u8 D_80147620[];
u8 func_800C57CC(void *rng, s32 limit);
u8 func_800C57A0(void *rng);
s32 func_800C5844(void *rng, u8 base, u8 top);
s32 func_800A3138(MapRect *room);
s32 func_800A315C(MapRect *room);
s32 func_800BB22C(void *floor, MapRect *room);
s32 func_800BAFE4(void *floor, MapRect *room);
void func_800B1B58(MapPos *pos, u16 kind);
void *func_800A2594(MapPos *out, MapPos *from, ShirenDirection_800BCF64 dir);
}



extern "C" s32 func_800BCF64(Floor_800BCF64 *floor)
{
    s32 width = 0;
    s32 height = 0;
    s32 tries;

    if (!(floor->flags & 0x20)) {
        return 0;
    }
    Room_800BCF64 *room = 0;
    for (tries = 100; --tries != -1;) {
        u8 index = func_800C57CC(D_80147620, (u8)(floor->room_count - 1));
        if (floor->room_open[index] == 0) {
            continue;
        }
        room = &D_801431F0[index];
        width = func_800A3138(&room->geometry);
        height = func_800A315C(&room->geometry);
        if (width < 5 || height < 5) {
            if (width < 5) {
                if (func_800BB22C(floor, &room->geometry) != 0) {
                    width++;
                }
            }
            if (height < 5) {
                if (func_800BAFE4(floor, &room->geometry) != 0) {
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
    MapPos center;
    s32 y = room->geometry.min.y + width / 2;
    center.y = y;
    if (!(width & 1)) {
        s32 odd = func_800C57A0(D_80147620) & 1;
        s32 adjusted = y;
        if (odd) adjusted--;
        center.y = adjusted;
    }
    s32 x = room->geometry.min.x + height / 2;
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
        MapPos pos;
        func_800A2594(&pos, &center, ShirenDirection_800BCF64(slot));
        func_800B1B58(&pos, floor->field_402);
    }
    return 1;
}
