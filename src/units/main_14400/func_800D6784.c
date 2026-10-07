#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x; s32 y; } Pos800D6784;
typedef struct { Pos800D6784 cur; Pos800D6784 start; Pos800D6784 end; } Range800D6784;
typedef struct { Pos800D6784 min; Pos800D6784 max; } Rect800D6784;

extern Rect800D6784 *D_80148080;
extern u8 D_80148000[];
extern u8 D_80148008[];
void *func_800A3610(void *out, void *it);
u32 func_800B1C6C(Pos800D6784 *pos);
s32 func_800D6AF8(void *map, void *filter, Pos800D6784 *pos);
s32 func_800D6D04(void *map, Pos800D6784 *pos, Pos800D6784 *best);

s32 func_800D6784(Pos800D6784 *best, u8 *flag) {
    Rect800D6784 *rect = D_80148080;
    Range800D6784 range;
    Pos800D6784 pos;
    s32 better;

    best->x = 0;
    best->y = 0;
    pos.x = rect->min.x;
    pos.y = rect->min.y;
    range.start = pos;
    range.cur = range.start;
    pos.x = rect->max.x;
    pos.y = rect->max.y;
    range.end = pos;
    for (;;) {
        Range800D6784 *r = &range;
        s32 more = r->cur.x <= r->end.x;

        if (!more) {
            break;
        }
        func_800A3610(&pos, r);
        if (!(func_800B1C6C(&pos) & 0x800)) {
            continue;
        }
        if (!func_800D6AF8(D_80148000, D_80148008, &pos)) {
            continue;
        }
        better = 0;
        if ((best->y | best->x) == 0 || func_800D6D04(D_80148000, &pos, best)) {
            better = 1;
        }
        if (!better) {
            continue;
        }
        *best = pos;
        *flag = 0;
    }
    return (best->y | best->x) != 0;
}
