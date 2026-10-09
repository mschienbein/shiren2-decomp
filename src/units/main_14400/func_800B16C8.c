#include "common.h"

typedef struct Pos800B16C8 {
    s32 x;
    s32 y;
} Pos800B16C8;

/* Rectangle walker consumed by func_800A3610 (cursor, row start, end corner). */
typedef struct Iter800B16C8 {
    Pos800B16C8 cur;
    Pos800B16C8 start;
    Pos800B16C8 end;
} Iter800B16C8;

/* 16-byte map bounds rectangle at 0x801429D0: start then end corner. */
typedef struct Bounds800B16C8 {
    Pos800B16C8 start;
    Pos800B16C8 end;
} Bounds800B16C8;

typedef struct Obj Obj;

extern Bounds800B16C8 D_801429D0;
Pos800B16C8 *func_800A3610(Pos800B16C8 *out, Iter800B16C8 *it);
void *func_800B4D80(Pos800B16C8 *p);
s32 func_800AC670(Obj *obj);
void *func_800B4E18(Pos800B16C8 *pos);

static inline void bounds_start(Pos800B16C8 *pos, Bounds800B16C8 *bounds) {
    pos->x = bounds->start.x;
    pos->y = bounds->start.y;
}

static inline void bounds_end(Pos800B16C8 *pos, Bounds800B16C8 *bounds) {
    pos->x = bounds->end.x;
    pos->y = bounds->end.y;
}

void func_800B16C8(void) {
    Iter800B16C8 range;
    Iter800B16C8 *r = &range;
    Pos800B16C8 pos;

    bounds_start(&pos, &D_801429D0);
    range.start = pos;
    range.cur = range.start;
    bounds_end(&pos, &D_801429D0);
    range.end = pos;
    for (;;) {
        s32 valid = range.cur.x <= r->end.x;
        Obj *obj;

        if (!valid) {
            break;
        }
        func_800A3610(&pos, r);
        obj = func_800B4D80(&pos);
        if (obj != 0 && func_800AC670(obj) != 0) {
            func_800B4E18(&pos);
        }
    }
}
