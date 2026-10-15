#include "common.h"
#include "map_types.h"

/*
 * Grow the room next to a position outward by whole free strips, then retile and
 * re-register it (C++ TU, g++ 2.8.1). Author: Anthropic claude-opus-5-5, xhigh.
 *
 * Contract note: the by-value func_800A2594 declaration below is one of the declaration
 * differences admitted by scratch/omp/rulings/ABI-IDENTICAL-DECLARATIONS.md (see the comment
 * at the declaration).
 *
 * The four cell rejections are one short-circuit chain assigned to `bad`: GCC sets it
 * to 0, branches each passing test to a single shared `bad = 1`, and reorg later copies
 * that store into every branch-likely delay slot.
 */

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { MapPos min, max; } Rect_800B38F8;
/* Room record (D_801431F0, stride 20): bounds followed by four side exit counts. */
typedef struct { Rect_800B38F8 area; u8 exits[4]; } Room_800B38F8;
/* Scan direction byte bound to func_800C25D0's const reference (no 0..7 mask, unlike
   MapDir); its constructor stores through the temporary's address register. */
struct Line_800B38F8 {
    u8 value;
    Line_800B38F8(s32 d) : value(d) {}
};
/* func_800C25D0/func_800C2758 line iterator: position, direction, count, index. */
typedef struct {
    MapPos position;
    u8 direction;
    s32 count;
    s32 index;
} LineIter_800B38F8;
/* func_800A3610 rectangle iterator: current cell, row origin, last cell. */
struct RectIter_800B38F8 {
    MapPos cur, origin, end;
    bool done() const { return cur.x > end.x; }
};
/* D_80143330 records (stride 24) refer to their room at +4. */
typedef struct {
    u8 pad0[4];
    Room_800B38F8 *room;
    u8 pad8[0x10];
} Trap_800B38F8;

extern "C" {
extern u8 D_8014344C;
extern Room_800B38F8 D_801431F0[16];
extern u8 D_80143448;
extern Trap_800B38F8 D_80143330[2];
s32 func_800B69F4(Room_800B38F8 *room, void *obj);
void func_800C25D0(LineIter_800B38F8 *iter, MapPos *start, const Line_800B38F8 &dir, s32 length);
void *func_800C2758(MapPos *out, LineIter_800B38F8 *iter);
u32 func_800B1C6C(const MapPos &pos);
/* ABI-identical declaration (ruling ABI-IDENTICAL-DECLARATIONS.md, scoped debt). The
   definition src/units/main_14400/func_800A2594.c spells the contract with an explicit
   out-pointer: void *func_800A2594(Tmp800A46BC *out, void *arg, Dir cell), returning out.
   This by-value view passes the hidden struct-return pointer in a0 and gets it back in v0,
   the same registers, so both views are one ABI. Mechanism: GCC 2.8.1 takes a by-value call's
   hidden-result address before it evaluates the arguments (calls.c 929-935), while explicit
   arguments are evaluated last to first (calls.c 990-993, 1632-1677). The ROM loads the
   neighbour temporary's address (sp+0x38) before (side * 2) & 7, so only this view matches:
   0 differing words here; the best explicit-out form differs in 7 words (1272 B,
   opus-06/a2594-r2/b38/func_800B38F8.best_explicit_out.cpp, 9ebda760...). */
MapPos func_800A2594(MapPos *from, MapDir dir);
void *func_800A3610(MapPos *out, RectIter_800B38F8 *iter);
void func_800B1BE0(MapPos *pos, s32 flags);
void func_800B1B58(MapPos *pos, u16 tile);
void func_800B6590(Room_800B38F8 *room, MapPos *min, MapPos *max);
void func_800D28D8(Trap_800B38F8 *trap);
void func_800B4A70(Rect_800B38F8 *rect);
void *func_800C5F60(void);
s32 func_80049CB4(s32 command, ...);
void func_800E2298(void *message);
}

extern "C" s32 func_800B38F8(void *obj)
{
    u8 i;
    s32 found;
    s32 side;
    Room_800B38F8 *room;
    s32 top;
    s32 left;
    s32 bottom;
    s32 right;
    s32 dir;
    s32 length;
    bool blocked;
    s32 steps;
    s32 j;
    u16 tile;
    MapPos start;
    LineIter_800B38F8 iter;
    Trap_800B38F8 *trap;
    void *message;

    /* Original entry 0x800B3910 (2412ffff) defines side-register s2 as -1 before the
       zero-count branch; on the nonempty path 0x800B394C (0240882d) copies that constant to
       s1, 0x800B3970 replaces s2 with func_800B69F4's result and 0x800B3974 compares it with
       s1. The initial source value is otherwise dead: evidenced original work and its
       compiler constant reuse, not a failure-path consumer. */
    side = -1;
    found = 0;
    /* Original 0x800B3918 (0000802d) zeroes found-register s0 and 0x800B3920 (0200b02d)
       copies that zero into room-register s6. It is never read (0x800B3964 overwrites it;
       the zero-count return restores incoming s6): original dead register work. */
    room = 0;
    for (i = 0; i < D_8014344C; i++) {
        room = &D_801431F0[i];
        side = func_800B69F4(room, obj);
        if (side != -1) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return 0;
    }
    dir = 0;
    length = 0;
    top = room->area.min.y;
    left = room->area.min.x;
    bottom = room->area.max.y;
    right = room->area.max.x;
    switch (side) {
        case 0:
            dir = 6;
            start.x = left;
            length = right - left + 1;
            start.y = bottom + 1;
            break;
        case 1:
            dir = 0;
            length = bottom - top + 1;
            start.y = top;
            start.x = left - 1;
            break;
        case 2:
            dir = 6;
            start.x = left;
            length = right - left + 1;
            start.y = top - 1;
            break;
        case 3:
            dir = 0;
            length = bottom - top + 1;
            start.y = top;
            start.x = right + 1;
            break;
    }
    steps = 0;
    iter.index = 0;
    iter.count = 0;
    while (1) {
        func_800C25D0(&iter, &start, Line_800B38F8(dir), length);
        blocked = false;
        while (iter.index < iter.count) {
            MapPos position;
            func_800C2758(&position, &iter);
            bool bad = (func_800B1C6C(position) & 0x4000) || (func_800B1C6C(position) & 0x2000) ||
                       (func_800B1C6C(position) & 0x80) ||
                       (func_800B1C6C(func_800A2594(&position, MapDir(side * 2))) & 0x1000);
            if (bad) {
                blocked = true;
                break;
            }
        }
        if (blocked) {
            break;
        }
        steps++;
        switch (side) {
            case 0:
                start.y++;
                break;
            case 1:
                start.x--;
                break;
            case 2:
                start.y--;
                break;
            case 3:
                start.x++;
                break;
        }
    }
    if (steps == 0) {
        return 0;
    }
    Rect_800B38F8 rect;
    rect.min.x = left;
    rect.min.y = top;
    rect.max.y = bottom;
    rect.max.x = right;
    switch (side) {
        case 0:
            rect.max.y = bottom + steps;
            break;
        case 1:
            rect.min.x = left - steps;
            break;
        case 2:
            rect.min.y = top - steps;
            break;
        case 3:
            rect.max.x = right + steps;
            break;
    }
    RectIter_800B38F8 fill;
    MapPos cell;
    tile = (i & 0xF) | 0x1000;
    fill.origin = rect.min;
    fill.cur = fill.origin;
    fill.end = rect.max;
    while (!fill.done()) {
        func_800A3610(&cell, &fill);
        func_800B1BE0(&cell, 0x800);
        func_800B1B58(&cell, tile);
    }
    func_800B6590(room, &rect.min, &rect.max);
    j = 0;
    trap = D_80143330;
    while (1) {
        if (j >= D_80143448) {
            break;
        }
        if (trap->room == room) {
            func_800D28D8(trap);
        }
        trap++;
        j++;
    }
    Rect_800B38F8 area;
    area.min = rect.min;
    area.max = rect.max;
    func_800B4A70(&area);
    message = func_800C5F60();
    func_80049CB4(0xA9, message);
    func_800E2298(message);
    return 1;
}
