#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos800B2DEC;
typedef struct { Pos800B2DEC min; Pos800B2DEC max; } Rect800B2DEC;
typedef struct { Pos800B2DEC cur; Pos800B2DEC start; Pos800B2DEC end; } Iter800B2DEC;
typedef struct { u16 cells[0x4C]; } Row800B2DEC;
extern Row800B2DEC D_80143450[];
void *func_800A3610(void *out, void *it);
s32 func_80049CB4(s32 id, ...);
void func_800B2DEC(Rect800B2DEC *rect) {
    Iter800B2DEC it;
    Iter800B2DEC *iter = &it;
    Pos800B2DEC pos;
    pos.x = rect->min.x;
    pos.y = rect->min.y;
    it.start = pos;
    it.cur = it.start;
    pos.x = rect->max.x;
    pos.y = rect->max.y;
    it.end = pos;
    while (1) {
        s32 more = it.cur.x <= iter->end.x;
        if (!more) break;
        func_800A3610(&pos, iter);
        D_80143450[pos.x].cells[pos.y] |= 0x400;
    }
    func_80049CB4(0xDC, rect);
}
