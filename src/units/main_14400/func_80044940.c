#include "common.h"

typedef struct { s32 x; s32 y; } Vec2;
typedef struct { s32 kind_00; Vec2 position_04; } Record;
extern Record D_80138B40;
extern s32 func_80044994(Record *record, Vec2 *position);

static __inline__ void zero_position(Vec2 *position)
{
    position->x = 0;
    position->y = 0;
}

Record *func_80044940(Record *record)
{
    Vec2 position;
    Record *result;
    zero_position(&position);
    if (func_80044994(record, &position)) {
        record->position_04 = position;
        result = record;
    } else {
        result = &D_80138B40;
    }
    return result;
}
