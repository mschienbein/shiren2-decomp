#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair minimum, maximum; } Rect;
typedef struct { unsigned char direction; } Direction;

extern void *func_800A27A4(Direction *direction, Pair *from, Pair *to);
extern s32 func_800A23E8(Pair *from, Pair *to);
extern s32 func_800A99D0(void);
extern Pair *func_800C5F60(void);
extern s32 func_800A31C8(Rect *rect, Pair *point);
extern void func_800A2758(Pair *point, Direction direction);
/* obj: receiver supplied by the original caller func_800C3754 (forwarded unchanged at
 * 0x800C3770); this function never reads it. */
s32 func_800C3114(void *obj, Pair *from, Pair *to, Pair *first, Pair *last)
{
    Pair point, center;
    Rect bounds;
    Direction direction;
    s32 radius;
    s32 state, remaining;
    Pair *origin;
    Direction *direction_ptr = &direction;
    point.x = from->x; point.y = from->y;
    center.x = to->x; center.y = to->y;
    func_800A27A4(direction_ptr, from, &center);
    radius = 5;
    center.x = to->x; center.y = to->y;
    remaining = func_800A23E8(from, &center) + 1;
    if (func_800A99D0() && !((D_80142F18.flags >> 2) & 1)) radius = 6;
    state = 0;
    origin = func_800C5F60();
    center.x = origin->x; center.y = origin->y;
    bounds.minimum.x = center.x - 5;
    bounds.minimum.y = center.y - radius;
    bounds.maximum.x = center.x + 5;
    bounds.maximum.y = center.y + radius;
    for (;;) {
        s32 inside;
        if (--remaining == -1) break;
        inside = func_800A31C8(&bounds, &point);
        if (!state && inside) {
            *first = point;
            state = 1;
        } else if (state == 1 && (!inside || !remaining)) {
            *last = point;
            state = 2;
            break;
        }
        func_800A2758(&point, direction);
    }
    return state == 2;
}
