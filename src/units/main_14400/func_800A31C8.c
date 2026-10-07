#include "common.h"
typedef struct { s32 x0, y0, x1, y1; } Rect;
typedef struct { s32 x, y; } Point;
s32 func_800A31C8(Rect *a, Point *b) { return b->y >= a->y0 && b->y <= a->y1 && b->x >= a->x0 && b->x <= a->x1; }
