#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { unsigned char value; } Direction;
typedef struct { Point first, last; } Rect;
typedef struct { Point current, first, last; } Iterator;
typedef struct { u32 bits; } Flags;
typedef union {
    Rect intersection;
    struct { Point nearest, distancePoint; } search;
} SearchScratch;
typedef struct { Point position; Direction direction; unsigned char pad09[0x17]; Flags flags; } Unit;
extern s32 func_800B56F0(void *point);
extern void *func_800A7DEC(void *unit);
extern s32 func_800A4CC4(void *unit, void *target, void *direction);
extern void *func_800A2594(Point *out, void *point, Direction direction);
extern void func_800A2F80(Direction *direction, s32 amount);
extern void *func_800B1F90(void *point);
extern char *func_800A7DE4(Unit *unit);
extern s32 func_800A4EFC(void *unit, void *direction);
extern void *func_800A2FD0(void *out, void *point, unsigned char radius);
extern Rect *func_800A324C(Rect *out, Rect *left, Rect *right);
extern s32 func_800A34D0(Rect *rect);
extern Point *func_800A3610(Point *out, Iterator *iterator);
extern s32 func_800A4314(Unit *unit, Point *point);
extern s32 func_800A23E8(Point *origin, Point *point);
extern s32 func_800E65C0(Unit *unit, Point *point, s32 mode);
static inline Point *copyPoint(Point *out, Point *point) {
    out->x = point->x;
    out->y = point->y;
    return out;
}
static inline Flags *getFlags(Flags *flags, Unit *unit) {
    flags->bits = unit->flags.bits;
    return flags;
}
static inline s32 canStep(Unit *unit, Direction *direction) {
    return func_800A4CC4(unit, func_800A7DEC(unit), direction);
}
static inline s32 hasCurrent(Iterator *iterator) {
    s32 current = iterator->current.x;
    return (iterator->last.x < current) ^ 1;
}
s32 func_800E80B8(Unit *unit) {
    Point origin, position;
    Rect bounds;
    SearchScratch scratch;
    Iterator iterator;
    Direction direction;
    Flags savedFlags;
    s32 remaining;
    Rect *area;
    s32 distance;
    if ((func_800B56F0(unit) ^ 1) != 0) return 0;
    copyPoint(&origin, &unit->position);
    direction = unit->direction;
    remaining = 8;
    for (;;) {
        s32 found = 0;
        if (--remaining == -1) break;
        if (canStep(unit, &direction)) {
            func_800A2594(&position, &origin, direction);
            found = func_800B56F0(&position) == 0;
        }
        if (found) return func_800A4EFC(unit, &direction);
        func_800A2F80(&direction, 1);
    }
    area = func_800B1F90(&origin);
    if (!area) return func_800A4EFC(unit, func_800A7DE4(unit));
    func_800A2FD0(&bounds, &origin, 5);
    func_800A324C(&scratch.intersection, &bounds, area);
    if ((func_800A34D0(&bounds) ^ 1) != 0) return 0;
    distance = 255;
    copyPoint(&position, &bounds.first);
    iterator.first = position;
    iterator.current = iterator.first;
    copyPoint(&position, &bounds.last);
    iterator.last = position;
    for (;;) {
        s32 found;
        s32 candidateDistance;
        if (!hasCurrent(&iterator)) break;
        func_800A3610(&position, &iterator);
        found = 0;
        if (!func_800B56F0(&position)) found = func_800A4314(unit, &position) != 0;
        if (found) {
            candidateDistance = func_800A23E8(&origin, copyPoint(&scratch.search.distancePoint, &position));
            if (candidateDistance < distance) {
                scratch.search.nearest = position;
                distance = candidateDistance;
            }
        }
    }
    if (distance != 255) {
        s32 result;
        s32 restore;
        restore = (getFlags(&savedFlags, unit)->bits & 0x04000000) != 0;
        unit->flags.bits &= ~0x04000000;
        result = func_800E65C0(unit, &scratch.search.nearest, 0);
        if (restore) unit->flags.bits |= 0x04000000;
        return result;
    }
    return 0;
}
