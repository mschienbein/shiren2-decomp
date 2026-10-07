#include "common.h"
typedef struct { s32 x; s32 y; } Vec2i;
s32 func_800A2854(Vec2i *p, Vec2i *min, Vec2i *max) {
    return p->x >= min->x && p->y >= min->y && p->x <= max->x && p->y <= max->y;
}
