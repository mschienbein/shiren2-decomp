#include "map_position_iterator.h"
typedef struct { MapPos origin; s32 field8; MapPos start, end; } Object;
extern s32 func_800FA710(Object *, MapPos *);
extern s32 func_800A23E8(MapPos *, MapPos *);
static inline void copy(MapPos *dest, const MapPos *source) {
    dest->x = source->x;
    dest->y = source->y;
}
static inline s32 in_range(MapPositionIterator *iter) { return iter->cur.x <= iter->end.x; }
MapPos *func_800FA760(MapPos *result, Object *obj) {
    MapPos best;
    MapPos origin;
    MapPositionIterator iter;
    MapPos current;
    MapPos distance_position;
    s32 best_distance = 0x4C;
    best.x = 0;
    best.y = 0;
    copy(&origin, &obj->origin);
    copy(&current, &obj->start);
    iter.start = current;
    iter.cur = iter.start;
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
