#include "common.h"

typedef unsigned char u8;

struct Vec2i {
    s32 x;
    s32 y;
    Vec2i() {}
    Vec2i(s32 px, s32 py) : x(px), y(py) {}
    Vec2i(const Vec2i &other) : x(other.x), y(other.y) {}
};

struct Rect {
    Vec2i start;
    Vec2i end;
};

struct RectIter {
    Vec2i cur;
    Vec2i start;
    Vec2i end;
    void setStart(Vec2i p) { start = p; cur = start; }
    void setEnd(Vec2i p) { end = p; }
    int valid() { return cur.x <= end.x; }
};

struct Tile {
    u8 kind;
    u8 feature;
};

extern "C" {
extern Rect D_801429C0;
Vec2i *func_800A3610(Vec2i *out, RectIter *it);
s32 func_800B4F74(Vec2i *pos);
void *func_800B4D80(Vec2i *pos);
}

extern "C" Vec2i func_800B37E8(void)
{
    RectIter iter;

    iter.setStart(D_801429C0.start);
    iter.setEnd(D_801429C0.end);
    while (iter.valid()) {
        Vec2i pos;
        s32 found;
        func_800A3610(&pos, &iter);
        found = 0;
        if (func_800B4F74(&pos)) {
            found = ((Tile *)func_800B4D80(&pos))->feature == 0xCE;
        }
        if (found) {
            return pos;
        }
    }
    return Vec2i(0, 0);
}
