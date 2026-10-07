#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pt;
typedef struct { Pt a; Pt b; } Rect;
typedef struct { u8 value; } Dir;
typedef struct { Pt cur; Pt start; Pt end; } Range;
void *func_800A2594(void *out, void *from, Dir dir);
void *func_800A3610(void *out, void *it);
void func_800B1B58(Pt *pos, u16 flags);
static __inline__ void copyPt(Pt *dst, Pt *src) {
    dst->x = src->x;
    dst->y = src->y;
}
static __inline__ s32 rangeActive(Range *r) { return r->cur.x <= r->end.x; }
void func_800D3F44(Rect **arg0) {
    Range range;
    Pt first;
    Pt srcA;
    Pt last;
    Pt srcB;
    Dir modes[2];

    copyPt(&srcA, &(*arg0)->a);
    modes[0].value = 7;
    func_800A2594(&first, &srcA, modes[0]);
    copyPt(&srcB, &(*arg0)->b);
    modes[1].value = 3;
    func_800A2594(&last, &srcB, modes[1]);
    range.start = first;
    range.cur = range.start;
    range.end = last;
    while (rangeActive(&range)) {
        func_800A3610(&first, &range);
        func_800B1B58(&first, 0x2000);
    }
}