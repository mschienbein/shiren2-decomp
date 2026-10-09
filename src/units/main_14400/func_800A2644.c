#include "common.h"
typedef struct { s32 x, y; } Position;
extern s32 func_800A23E8(Position *from, Position *to);
Position *func_800A2644(Position *out, Position *from, Position *to, s32 range) {
    Position result, target;
    s32 delta, distance;
    target.x = to->x;
    target.y = to->y;
    distance = func_800A23E8(from, &target);
    range <<= 7;
    range /= distance;
    delta = (to->y - from->y) * range;
    if (delta < 0) delta -= 127; else delta += 127;
    result.y = from->y + delta / 128;
    delta = (to->x - from->x) * range;
    if (delta < 0) delta -= 127; else delta += 127;
    result.x = from->x + delta / 128;
    out->x = result.x;
    out->y = result.y;
    return out;
}
