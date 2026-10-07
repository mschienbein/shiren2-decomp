#include "common.h"

typedef struct { s32 x; s32 y; } Pos800B4EB8;
typedef struct { Pos800B4EB8 cur; Pos800B4EB8 start; Pos800B4EB8 end; } Range800B4EB8;

typedef struct { Pos800B4EB8 start; Pos800B4EB8 end; } Rect;
extern Rect D_801429C0;
void *func_800A3610(void *out, void *range);
void func_800B4E7C(Pos800B4EB8 *pos);

static inline void range_set_start(Range800B4EB8 *r, Pos800B4EB8 *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    r->start = *p;
    r->cur = r->start;
}
static inline void range_set_end(Range800B4EB8 *r, Pos800B4EB8 *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    r->end = *p;
}
void func_800B4EB8(void) {
    Range800B4EB8 range;
    Pos800B4EB8 pos;

    range_set_start(&range, &pos, D_801429C0.start.x, D_801429C0.start.y);
    range_set_end(&range, &pos, D_801429C0.end.x, D_801429C0.end.y);
    for (;;) {
        Range800B4EB8 *r = &range;
        s32 more = r->cur.x <= r->end.x;

        if (!more) {
            break;
        }
        func_800A3610(&pos, r);
        func_800B4E7C(&pos);
    }
}
