#include "common.h"

typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Point;

typedef struct {
    s32 x;
    s32 y;
    unsigned char pad8[4];
    s16 dy;
    s16 dx;
} Rect;

Point *func_800C52EC(Point *out, Rect *rect) {
    s32 dx = rect->dx;
    s32 dy = rect->dy;
    s32 x = rect->x;
    s32 y = rect->y;
    out->x = x + dx;
    out->y = y + dy;
    return out;
}
