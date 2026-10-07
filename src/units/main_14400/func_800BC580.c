#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { Point current, start, end; } Iterator;
typedef struct { char pad[0x400]; signed char field_400; unsigned short field_402; } Obj;
extern unsigned char D_80142EFC;
extern char D_80147620[];
typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect;
extern Rect D_801429C0;
extern s32 func_800C587C(void *, unsigned char);
extern u32 func_800B1C6C(void *pos);
extern void *func_800A3610(void *out, void *it);
extern void func_800B1AE0(void *pos, unsigned short value);
static inline Point *setPoint(Point *p, s32 x, s32 y) { p->x = x; p->y = y; return p; }
static inline void initIterator(Iterator *p, Point *temp, s32 x0, s32 y0, s32 x1, s32 y1) {
    p->start = *setPoint(temp, x0, y0); p->current = p->start; p->end = *setPoint(temp, x1, y1);
}
void func_800BC580(Obj *a) {
    Iterator iter; Point temp; Iterator *it; Point *p;
    if (a->field_400 == 1) return;
    if (a->field_400 == 2) return;
    if (a->field_400 == 3) return;
    if (a->field_402 == 0x80) return;
    { s32 failed = func_800C587C(D_80147620, D_80142EFC) != 1; if (failed) return; }
    it = &iter;
    initIterator(it, &temp, D_801429C0.x0, D_801429C0.y0, D_801429C0.x1, D_801429C0.y1);
    p = &temp;
    for (;;) { s32 valid = iter.current.x <= it->end.x; if (!valid) break;
        func_800A3610(p, it);
        if (func_800B1C6C(p) & 0x4000) func_800B1AE0(p, 0x2000);
    }
}
