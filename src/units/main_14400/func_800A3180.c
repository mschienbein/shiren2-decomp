#include "common.h"

typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect800A3180;

void func_800A3180(Rect800A3180 *rect) {
    s32 tmp;
    if (rect->y0 > rect->y1) {
        tmp = rect->y0 - 1;
        rect->y0 = rect->y1 + 1;
        rect->y1 = tmp;
    }
    if (rect->x0 > rect->x1) {
        tmp = rect->x0 - 1;
        rect->x0 = rect->x1 + 1;
        rect->x1 = tmp;
    }
}
