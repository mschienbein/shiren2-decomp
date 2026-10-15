#include "common.h"
#include "map_position_iterator.h"

/*
 * Walk toward the nearest 0x2000-flagged cell of the unit's area (C++ TU, g++ 2.8.1).
 * The original copies each position handed to func_800A23E8/func_800A27A4
 * member-wise into a separate temporary. The scan's distance copy is at
 * sp+0x40; bound copies, scan output and later block-scope objects reuse
 * sp+0x38. Keep their source scopes and the shared MapPos copy constructor.
 * func_800A23E8 subtracts the origin from its target in place, so it receives
 * a non-const temporary copy, never the live current, best or iterator state.
 * func_800A27A4 receives its separate copy by const reference (pointer ABI).
 */

typedef unsigned char u8;

struct Dir_800E7EB4 {
    u8 v;
};

/* Partial foreign-object view; these offsets do not establish its lifetime. */
struct Obj_800E7EB4 {
    MapPos pos;         /* 0x00 */
    char pad8[4];
    MapPos min;         /* 0x0C */
    MapPos max;         /* 0x14 */
    char pad1C[0x48];
    MapPos target;      /* 0x64 */
    char pad6C[6];
    u8 flags;           /* 0x72 */
};

extern "C" {
s32 func_800A23E8(MapPos *origin, MapPos *pos);
u32 func_800B1C6C(MapPos *pos);
void *func_800A27A4(Dir_800E7EB4 *out, MapPos *from, const MapPos &to);
void *func_800A2594(MapPos *out, MapPos *from, Dir_800E7EB4 dir);
s32 func_800A4360(Obj_800E7EB4 *obj, MapPos *pos);
s32 func_800A422C(Obj_800E7EB4 *obj, MapPos *pos, s32 arg);
s32 func_800E66EC(Obj_800E7EB4 *obj);
}

static inline s32 MapPositionIterator_valid(MapPositionIterator *it) { return it->cur.x <= it->end.x; }
static inline s32 isPositive(s32 v) { return v > 0; }

/* Both callers bind this reference to a fresh, non-const MapPos temporary.
 * Its lifetime lasts through the enclosing distance-call full-expression. */
static inline MapPos *scratch(const MapPos &copy) { return const_cast<MapPos *>(&copy); }

extern "C" s32 func_800E7EB4(Obj_800E7EB4 *obj)
{
    obj->flags |= 2;
    if ((obj->target.y | obj->target.x) != 0) {
        return func_800E66EC(obj);
    }
    MapPos cur(obj->pos);
    MapPos best;
    MapPositionIterator iter;
    iter.start = MapPos(obj->min);
    iter.cur = iter.start;
    iter.end = MapPos(obj->max);
    s32 bestDist = 1000;
    while (MapPositionIterator_valid(&iter)) {
        MapPos p;
        func_800A3610(&p, &iter);
        s32 dist = func_800A23E8(&cur, scratch(MapPos(p)));
        if ((func_800B1C6C(&p) & 0x2000) && dist < bestDist) {
            best = p;
            bestDist = dist;
        }
    }
    if (bestDist == 1000) {
        return 0;
    }
    s32 steps = func_800A23E8(&cur, scratch(MapPos(best))) * 2;
    while (isPositive(steps--)) {
        Dir_800E7EB4 dir;
        func_800A27A4(&dir, &cur, MapPos(best));
        MapPos next;
        func_800A2594(&next, &cur, dir);
        if (!func_800A422C(obj, &next, func_800A4360(obj, &next))) {
            continue;
        }
        cur = next;
        if (!(func_800B1C6C(&cur) & 0x2000)) {
            continue;
        }
        obj->target = cur;
        return func_800E66EC(obj);
    }
    return 0;
}
