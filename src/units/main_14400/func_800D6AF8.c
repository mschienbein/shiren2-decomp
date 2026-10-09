#include "common.h"
typedef struct { s32 x, y; } Pair;
static inline void difference(Pair *delta, const Pair *from, const Pair *to) {
    delta->y = to->y - from->y;
    delta->x = to->x - from->x;
}
static inline void absolute(Pair *dest, const Pair *source) {
    dest->y = source->y < 0 ? -source->y : source->y;
    dest->x = source->x < 0 ? -source->x : source->x;
}
s32 func_800D6AF8(Pair *from, unsigned char *direction, Pair *to) {
    Pair delta, magnitude;
    s32 dir;
    difference(&delta, from, to);
    absolute(&magnitude, &delta);
    dir = *direction;
    switch (dir) {
    case 2:
        return delta.x < 0 && magnitude.y * 100 <= magnitude.x * 50;
    case 6:
        return delta.x > 0 && magnitude.y * 100 <= magnitude.x * 50;
    case 4:
        return delta.y < 0 && magnitude.x * 100 <= magnitude.y * 50;
    case 0:
        return delta.y > 0 && magnitude.x * 100 <= magnitude.y * 50;
    default:
        if (magnitude.y * 100 < magnitude.x * 25) return 0;
        if (magnitude.x * 100 < magnitude.y * 25) return 0;
        if (delta.y > 0 && delta.x < 0 && dir == 1) return 1;
        if (delta.y < 0 && delta.x < 0 && dir == 3) return 1;
        if (delta.y > 0 && delta.x > 0 && dir == 7) return 1;
        if (delta.y < 0 && delta.x > 0 && dir == 5) return 1;
        return 0;
    }
}
