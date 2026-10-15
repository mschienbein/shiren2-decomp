#include "map_ring_iterator.h"

typedef struct Dir { unsigned char value; } Dir;
typedef struct { unsigned char unknown00[0x18]; short adjust18; short unknown1a; s32 (*method1c)(void *, s32); } VTable;
typedef struct { s32 unknown00[2]; VTable *field08; } Object;
extern s32 func_800AD714(Object *, MapPos *), func_800B56F0(MapPos *);
extern u32 func_800B1C6C(MapPos *);
extern void *func_800A27A4(unsigned char *, MapPos *, MapPos *);
extern void *func_800A2594(MapPos *, MapPos *, Dir);
s32 func_800ADD50(Object *object, MapPos *result) {
    MapPos origin, candidate, fallback;
    MapRingIterator iterator;
    MapPos current;
    Dir direction;
    s32 avoid;
    VTable *table;
    MapPos *start = &origin;
    origin.x = result->x;
    start->y = result->y;
    table = object->field08;
    avoid = table->method1c((char *)object + table->adjust18, 0x24);
    fallback.x = 0;
    fallback.y = 0;
    current.x = origin.x;
    current.y = start->y;
    func_800C5280(&iterator, &current, 2);
    while (func_800C559C(&iterator)) {
        s32 distance = iterator.radius;
        func_800C532C(&current, &iterator);
        candidate = current;
        if (!func_800AD714(object, &candidate)) continue;
        if (distance == 2) {
            current.x = candidate.x;
            current.y = candidate.y;
            func_800A27A4(&direction.value, &origin, &current);
            func_800A2594(&current, &origin, direction);
            if (func_800B1C6C(&current) & 0x4000) continue;
        }
        if (avoid && func_800B56F0(&candidate)) {
            if (!(fallback.y | fallback.x)) fallback = candidate;
        } else {
            *result = candidate;
            return 1;
        }
    }
    {
        s32 found;
        if (!(fallback.y | fallback.x)) {
            found = 0;
        } else {
            *result = fallback;
            found = 1;
        }
        return found;
    }
}
