#include "common.h"
#include "map_position_iterator.h"

typedef struct { MapRect bounds; unsigned char side_counts[4]; } Room;
/* Borrowed prefix of a 24-byte D_80143330 owner record; +4 points to the
 * complete 20-byte room, not only its rectangle. No owner is allocated here. */
typedef struct { signed char owner_00; unsigned char pad_01[3]; Room *field_04; } Object;
/* Borrowed entity prefix; its endpoint fields are handled by the callbacks. */
typedef struct {
    unsigned char field_00[0x80];
    Object *field_80;
} Entity;
/* Separate scalar entity cursor, not a position walker. */
typedef struct { s32 field_00; s32 field_04; } Iterator;
extern s32 func_800B68B0(Room *);
extern s32 func_800A9070(Iterator *, s32);
extern Entity *func_800A910C(Iterator *);
extern MapPos *func_800B6A98(MapPos *out, Room *room, s32 index);
extern s32 func_800D1E90(Object *, Entity *, MapPos *, MapPos *, MapPos *, s32);
extern void func_800F61B4(Entity *, MapPos *, MapPos *);
extern void func_800B1BE0(MapPos *, s32);
extern void func_800D23AC(Object *);

static inline void lower_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->min.x;
    point->y = bounds->min.y;
}

static inline void upper_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->max.x;
    point->y = bounds->max.y;
}

static inline s32 range_has_next(MapPositionIterator *range) {
    return range->cur.x <= range->end.x;
}

void func_800D28D8(Object *object) {
    /* Before walk initialization, cur/start/end serve as origin/out/pos. */
    MapPositionIterator range;
    MapPos point;
    Iterator iterator;
    s32 index = 0;
    s32 count = func_800B68B0(object->field_04);
    iterator.field_00 = 0;
    while (func_800A9070(&iterator, 0x57)) {
        Entity *entity = func_800A910C(&iterator);
        if (entity->field_80 == object) {
            do {
                func_800B6A98(&range.cur, object->field_04, index);
                if (func_800D1E90(object, entity, &range.cur, &range.start, &range.end, 0)) {
                    func_800F61B4(entity, &range.start, &range.end);
                    break;
                }
                index++;
            } while (index != count);
        }
    }
    {
        MapRect *bounds = &object->field_04->bounds;
        lower_point(&point, bounds);
        range.start = point;
        range.cur = range.start;
        upper_point(&point, bounds);
        range.end = point;
    }
    for (;;) {
        s32 valid = range_has_next(&range);
        if (!valid) break;
        func_800A3610(&point, &range);
        func_800B1BE0(&point, 0x10);
    }
    func_800D23AC(object);
}
