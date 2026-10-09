#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { s32 x, y; } Position;
typedef struct {
    Position origin;
    u8 direction;
    u8 limit;
    u8 index;
    u8 mode;
    s16 offset_y;
    s16 offset_x;
    s32 held;
    s16 ring;
} Iterator;

static inline void copy(Position *dest, const Position *source)
{
    dest->x = source->x;
    dest->y = source->y;
}

Position *func_800C282C(Position *result, Iterator *it)
{
    Position p;
    s32 t;

    p.y = it->offset_y;
    p.x = it->offset_x;
    switch (it->direction) {
    case 4:
        t = p.x;
        p.x = p.y;
        p.y = -t;
        break;
    case 0:
        t = p.x;
        p.x = p.y;
        p.y = t;
        break;
    case 3:
        p.y = -p.y;
    case 1:
    case 2:
        p.x = -p.x;
        break;
    case 5:
        p.y = -p.y;
        break;
    }
    p.y += it->origin.y;
    p.x += it->origin.x;
    copy(result, &p);
    return result;
}
