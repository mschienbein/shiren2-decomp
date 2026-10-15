#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[3]; u8 col_3; u8 pad4[3]; u8 row_7; } Cell800C012C;
extern MapPos D_801C9BC0;
extern u8 D_80147620[];
s32 func_800C5844(void *table, u8 a, u8 b);
s32 func_800C00B4(void *ctx, const MapRect *rect);

static inline void setPoint800C012C(MapPos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
}

static inline void copyPoint800C012C(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline void initRect800C012C(MapPos *a, MapPos *b, s32 left, s32 top, s32 right, s32 bottom) {
    setPoint800C012C(a, left, top);
    b->x = right;
    b->y = bottom;
}

static inline void copyRect800C012C(MapRect *dst, MapRect *src) {
    copyPoint800C012C(&dst->min, &src->min);
    copyPoint800C012C(&dst->max, &src->max);
}

/* ctx is never reassigned (it plays the role of g++'s `this`, a const pointer):
   the read-only parameter's stack slot is unchanging, so its reload before the
   final call may be scheduled ahead of the rectangle stores. */
s32 func_800C012C(void *const ctx, MapRect *out, Cell800C012C *from, Cell800C012C *to, u8 count) {
    MapRect rect;
    u8 i = 0;

    while (1) {
        s32 h;
        s32 w;
        s32 top;
        s32 left;

        if (!(i < count)) {
            return 0;
        }
        h = (u8)func_800C5844(D_80147620, from->row_7, to->row_7) + D_801C9BC0.y * 2;
        w = (u8)func_800C5844(D_80147620, from->col_3, to->col_3) + D_801C9BC0.x * 2;
        top = (u8)func_800C5844(D_80147620, 10 - D_801C9BC0.y, D_801C9BC0.y + 0x42 - h);
        left = (u8)func_800C5844(D_80147620, 10 - D_801C9BC0.x, D_801C9BC0.x + 0x2C - w);
        initRect800C012C(&rect.min, &rect.max, left, top, left + w - 1, top + h - 1);
        out->min = rect.min;
        out->max = rect.max;
        copyRect800C012C(&rect, out);
        if ((func_800C00B4(ctx, &rect) ^ 1) != 0) {
            return 1;
        }
        i++;
    }
}
