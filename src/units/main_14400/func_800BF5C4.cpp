#include "common.h"
typedef unsigned char u8;

struct Point {
    s32 x;
    s32 y;
    Point(s32 px, s32 py) : x(px), y(py) {}
};

struct Room {
    s32 x0, y0, x1, y1;
    u8 pad10[4];
};

struct Obj {
    u8 pad[0x3DC];
    s32 count;
};

extern "C" {
extern Room D_801431F0[];
extern u8 D_80147620[];
s32 func_800C587C(void *rng, u8 chance);
u32 func_800B1C6C(Point *pos);
void func_800BC69C(Obj *obj, Room *room, s32 mode);
}

static inline int bits_clear(u32 value, u32 mask)
{
    if (value & mask) {
        return 0;
    }
    return 1;
}

extern "C" void func_800BF5C4(Obj *obj)
{
    s32 i = 0;
    while (i < obj->count) {
        int ok = 0;
        Room *room = &D_801431F0[i];
        s32 y0 = room->y0;
        s32 x0 = room->x0;
        s32 y1 = room->y1;
        s32 x1 = room->x1;
        if (func_800C587C(D_80147620, 10)) {
            Point p1(x0, y0);
            if (bits_clear(func_800B1C6C(&p1), 0x4000)) {
                Point p2(x0, y1);
                if (bits_clear(func_800B1C6C(&p2), 0x4000)) {
                    Point p3(x1, y0);
                    if (bits_clear(func_800B1C6C(&p3), 0x4000)) {
                        Point p4(x1, y1);
                        ok = bits_clear(func_800B1C6C(&p4), 0x4000);
                    }
                }
            }
        }
        if (ok) {
            func_800BC69C(obj, room, 1);
        }
        i++;
    }
}
