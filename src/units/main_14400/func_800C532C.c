#include "map_ring_iterator.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern s8 D_801475F0[][2];
extern unsigned char D_80147620[];
s32 func_800C5A1C(void *rng, s32 base, s32 top);
static inline void offset_position(MapPos *pos, MapRingIterator *w, s32 dx, s32 dy) {
    s32 x = w->center.x;
    s32 y = w->center.y;
    s32 next_x = x + dx;
    s32 next_y = y + dy;
    pos->x = next_x;
    pos->y = next_y;
}

MapPos *func_800C532C(MapPos *out, MapRingIterator *w) {
    MapPos pos;
    s16 dx;
    s16 dy;

    offset_position(&pos, w, w->offsetX, w->offsetY);
    if (w->radius <= w->limit) {
        if (++w->steps >= w->radius * 8) {
            w->radius++;
            w->steps = 0;
            w->direction = func_800C5A1C(&D_80147620, 0, 3);
            if (w->direction == 0 || w->direction == 2) {
                w->offsetY = func_800C5A1C(&D_80147620, -w->radius, w->radius);
                w->offsetX = (w->direction == 0) ? -w->radius : w->radius;
            } else {
                w->offsetY = (w->direction == 1) ? w->radius : -w->radius;
                w->offsetX = func_800C5A1C(&D_80147620, -w->radius, w->radius);
            }
        }
        w->offsetY += D_801475F0[w->direction][0];
        w->offsetX += D_801475F0[w->direction][1];
        dy = (w->offsetY < 0) ? -w->offsetY : w->offsetY;
        dx = (w->offsetX < 0) ? -w->offsetX : w->offsetX;
        if (w->radius < dy || w->radius < dx) {
            w->offsetY -= D_801475F0[w->direction][0];
            w->offsetX -= D_801475F0[w->direction][1];
            w->direction = (w->direction < 3) ? w->direction + 1 : 0;
            w->offsetY += D_801475F0[w->direction][0];
            w->offsetX += D_801475F0[w->direction][1];
        }
    }
    out->x = pos.x;
    out->y = pos.y;
    return out;
}
