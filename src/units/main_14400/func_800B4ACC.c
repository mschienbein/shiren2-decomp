#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 x; s32 y; } Pos800B4ACC;
typedef struct { Pos800B4ACC cur; Pos800B4ACC start; Pos800B4ACC end; } Range800B4ACC;
typedef struct { Pos800B4ACC start; Pos800B4ACC end; } Rect;
extern Rect D_801429C0;
void *func_800A3610(void *out, void *range);
void func_800B4A1C(Pos800B4ACC *pos);
static inline void range_set_start(Range800B4ACC *r, Pos800B4ACC *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    r->start = *p;
    r->cur = r->start;
}
static inline void range_set_end(Range800B4ACC *r, Pos800B4ACC *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    r->end = *p;
}
void func_800B4ACC(void) {
    Range800B4ACC range;
    Range800B4ACC *r = &range;
    Pos800B4ACC pos;
    range_set_start(&range, &pos, D_801429C0.start.x, D_801429C0.start.y);
    range_set_end(&range, &pos, D_801429C0.end.x, D_801429C0.end.y);
    for (;;) {
        s32 valid = range.cur.x <= r->end.x;
        if (!valid) break;
        func_800A3610(&pos, r);
        func_800B4A1C(&pos);
    }
}
