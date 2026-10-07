#include "common.h"

typedef struct { s32 x; s32 y; } Pair;
typedef struct { Pair lower; Pair upper; } Bounds;
typedef struct { s32 field_00; Bounds *field_04; } Object;
typedef struct {
    unsigned char field_00[0x80];
    Object *field_80;
} Entity;
typedef struct { Pair current; Pair lower; Pair upper; } Range;
typedef struct { s32 field_00; s32 field_04; } Iterator;
extern s32 func_800B68B0(Bounds *);
extern s32 func_800A9070(Iterator *, s32);
extern Entity *func_800A910C(Iterator *);
extern void *func_800B6A98(void *out, void *room, s32 index);
extern s32 func_800D1E90(Object *, Entity *, Pair *, Pair *, Pair *, s32);
extern void func_800F61B4(Entity *, Pair *, Pair *);
extern void *func_800A3610(void *out, void *it);
extern void func_800B1BE0(Pair *, s32);
extern void func_800D23AC(Object *);

static inline void lower_point(Pair *point, Bounds *bounds) {
    point->x = bounds->lower.x;
    point->y = bounds->lower.y;
}

static inline void upper_point(Pair *point, Bounds *bounds) {
    point->x = bounds->upper.x;
    point->y = bounds->upper.y;
}

static inline s32 range_has_next(Range *range) {
    return range->current.x <= range->upper.x;
}

void func_800D28D8(Object *object) {
    Range range;
    Pair point;
    Iterator iterator;
    s32 index = 0;
    s32 count = func_800B68B0(object->field_04);
    iterator.field_00 = 0;
    while (func_800A9070(&iterator, 0x57)) {
        Entity *entity = func_800A910C(&iterator);
        if (entity->field_80 == object) {
            do {
                func_800B6A98(&range.current, object->field_04, index);
                if (func_800D1E90(object, entity, &range.current, &range.lower, &range.upper, 0)) {
                    func_800F61B4(entity, &range.lower, &range.upper);
                    break;
                }
                index++;
            } while (index != count);
        }
    }
    {
        Bounds *bounds = object->field_04;
        lower_point(&point, bounds);
        range.lower = point;
        range.current = range.lower;
        upper_point(&point, bounds);
        range.upper = point;
    }
    for (;;) {
        s32 valid = range_has_next(&range);
        if (!valid) break;
        func_800A3610(&point, &range);
        func_800B1BE0(&point, 0x10);
    }
    func_800D23AC(object);
}
