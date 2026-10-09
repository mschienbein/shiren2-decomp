#include "common.h"
typedef struct { s32 x, y; } Pair;
typedef struct { Pair origin; s32 field8; Pair start, end; } Object;
typedef struct { Pair current, start, end; } Iterator;
extern Pair *func_800A3610(Pair *, Iterator *);
extern s32 func_800FA710(Object *, Pair *);
extern s32 func_800A23E8(Pair *, Pair *);
static inline void copy(Pair *dest, const Pair *source) {
    dest->x = source->x;
    dest->y = source->y;
}
static inline s32 in_range(Iterator *iter) { return iter->current.x <= iter->end.x; }
Pair *func_800FA760(Pair *result, Object *obj) {
    Pair best;
    Pair origin;
    Iterator iter;
    Pair current;
    Pair distance_position;
    s32 best_distance = 0x4C;
    best.x = 0;
    best.y = 0;
    copy(&origin, &obj->origin);
    copy(&current, &obj->start);
    iter.start = current;
    iter.current = iter.start;
    copy(&current, &obj->end);
    iter.end = current;
    for (;;) {
        s32 distance;
        if (!in_range(&iter)) break;
        func_800A3610(&current, &iter);
        if (!func_800FA710(obj, &current)) continue;
        distance_position.x = current.x;
        distance_position.y = current.y;
        distance = func_800A23E8(&origin, &distance_position);
        if (distance < best_distance) {
            best = current;
            best_distance = distance;
        }
    }
    copy(result, &best);
    return result;
}
