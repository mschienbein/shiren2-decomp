#include "common.h"

/*
 * Dungeon-room painter (C++ TU). The original constructs each by-value Vec2i
 * argument with a member-wise copy constructor into one reused temporary slot
 * (sp+0x28) before the bitwise assignment into the iterator; C spellings of the
 * same copies leave the loop invariants allocated/scheduled differently.
 */

typedef unsigned short u16;

struct Vec2i {
    s32 x;
    s32 y;
    Vec2i() {}
    Vec2i(const Vec2i &other) : x(other.x), y(other.y) {}
};

struct Rect {
    Vec2i start;
    Vec2i end;
};

/* Row-major cell walk over [start, end]; func_800A3610 returns cur and advances. */
struct RectIter {
    Vec2i cur;
    Vec2i start;
    Vec2i end;
    void setStart(Vec2i p) { start = p; cur = start; }
    void setEnd(Vec2i p) { end = p; }
    int valid() { return cur.x <= end.x; }
};

/* 0x14-byte room record: bounds rectangle plus four per-side bytes (unused here). */
struct RoomRecord {
    Rect rect;
    unsigned char pad10[4];
};

extern "C" {
extern RoomRecord D_801431F0[];
void *func_800A3610(void *out, void *it);
void func_800B1BE0(Vec2i *pos, s32 mask);
void func_800B1B58(Vec2i *pos, u16 flags);
void func_800B6728(void *self, Rect *src);
}

/* self: caller-supplied object pointer, unused here. */
extern "C" void func_800B7948(void *self, s32 index, Rect *rect)
{
    RectIter iter;

    iter.setStart(rect->start);
    iter.setEnd(rect->end);
    while (iter.valid()) {
        Vec2i pos;
        func_800A3610(&pos, &iter);
        func_800B1BE0(&pos, 0x4000);
        func_800B1B58(&pos, (index & 0xF) | 0x1200);
    }
    func_800B6728(&D_801431F0[index], rect);
}
