#include "common.h"

typedef unsigned char u8;

struct Point {
    s32 x;
    s32 y;
    Point() {}
    Point(const Point &o) : x(o.x), y(o.y) {}
};

struct Rect {
    Point a;
    Point b;
    Rect() {}
    Rect(const Rect &o) : a(o.a), b(o.b) {}
    Rect &operator=(const Rect &o)
    {
        a = o.a;
        b = o.b;
        return *this;
    }
};

struct Floor {
    char pad[0x3DC];
    s32 room_count;
};

extern "C" {
extern u8 D_80143392;
extern u8 D_8014344C;
extern u8 D_80147620[];
/* Per-room bounds parameters have four-byte rows; rows 0..7 are used. */
extern const u8 D_80153DB4[][4];
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800B7948(void *self, s32 index, Rect *rect);
void func_800B7E9C(void *arg0, const Rect *arg1, const Rect *arg2);
void func_800B17A4(void);
}

/* func_800B7948 receives its rectangle by value (copy-constructed temporary). */
static inline void paint_room(Floor *floor, s32 index, Rect bounds)
{
    func_800B7948(floor, index, &bounds);
}

extern "C" void func_800C1290(Floor *floor)
{
    Rect room;
    Rect rooms[2];
    for (s32 i = 0; i < 2; i++) {
        room.a.y = (u8)func_800C5844(D_80147620, D_80153DB4[0][i], D_80153DB4[1][i]) + 10;
        room.a.x = (u8)func_800C5844(D_80147620, D_80153DB4[2][i], D_80153DB4[3][i]) + 10;
        room.b.y = (u8)func_800C5844(D_80147620, D_80153DB4[4][i], D_80153DB4[5][i]) + 10;
        room.b.x = (u8)func_800C5844(D_80147620, D_80153DB4[6][i], D_80153DB4[7][i]) + 10;
        rooms[i] = room;
        paint_room(floor, i, rooms[i]);
    }
    func_800B7E9C(floor, &rooms[0], &rooms[1]);
    floor->room_count = 2;
    D_8014344C = 2;
    func_800B17A4();
    D_80143392 = 0;
}
