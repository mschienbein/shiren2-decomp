#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { s32 x; s32 y; } Point800C012C;
typedef struct { Point800C012C a; Point800C012C b; } Rect800C012C;
typedef struct { u8 pad0[3]; u8 col_3; u8 pad4[3]; u8 row_7; } Cell800C012C;
extern Point800C012C D_801C9BC0;
extern u8 D_80147620[];
s32 func_800C5844(void *table, u8 a, u8 b);
s32 func_800C00B4(void *ctx, Rect800C012C *rect);

static inline void setPoint800C012C(Point800C012C *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
}

static inline void copyPoint800C012C(Point800C012C *dst, Point800C012C *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline void initRect800C012C(Point800C012C *a, Point800C012C *b, s32 left, s32 top, s32 right, s32 bottom) {
    setPoint800C012C(a, left, top);
    b->x = right;
    b->y = bottom;
}

static inline void copyRect800C012C(Rect800C012C *dst, Rect800C012C *src) {
    copyPoint800C012C(&dst->a, &src->a);
    copyPoint800C012C(&dst->b, &src->b);
}

/* ctx is never reassigned (it plays the role of g++'s `this`, a const pointer):
   the read-only parameter's stack slot is unchanging, so its reload before the
   final call may be scheduled ahead of the rectangle stores. */
s32 func_800C012C(void *const ctx, Rect800C012C *out, Cell800C012C *from, Cell800C012C *to, u8 count) {
    Rect800C012C rect;
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
        initRect800C012C(&rect.a, &rect.b, left, top, left + w - 1, top + h - 1);
        out->a = rect.a;
        out->b = rect.b;
        copyRect800C012C(&rect, out);
        if ((func_800C00B4(ctx, &rect) ^ 1) != 0) {
            return 1;
        }
        i++;
    }
}
