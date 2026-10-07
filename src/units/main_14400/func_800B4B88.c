#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { Pos cur; Pos begin; Pos end; } Range;
typedef struct { s32 unk[2]; } Cursor;
typedef struct { char pad[0x1E]; u8 flags; } Ent;
typedef struct { Pos start; Pos end; } Rect;
extern Rect D_801429C0;
void *func_800A3610(void *out, void *range);
Ent *func_800B4928(Cursor *);
void func_800B4A1C(Cursor *);
static inline s32 Range_valid(Range *r) { return r->cur.x <= r->end.x; }
static inline void Range_setBegin(Range *r) {
    Pos p;
    p.x = D_801429C0.start.x;
    p.y = D_801429C0.start.y;
    r->begin = p;
    r->cur = r->begin;
}
static inline void Range_setEnd(Range *r) {
    Pos p;
    p.x = D_801429C0.end.x;
    p.y = D_801429C0.end.y;
    r->end = p;
}
static inline void Range_init(Range *r) {
    Range_setBegin(r);
    Range_setEnd(r);
}
void func_800B4B88(void) {
    Range r;
    Range_init(&r);
    while (1) {
        Cursor c;
        Ent *e;
        s32 ok = Range_valid(&r);
        if (!ok) break;
        func_800A3610(&c, &r);
        e = func_800B4928(&c);
        if (e && (e->flags & 0xC)) continue;
        func_800B4A1C(&c);
    }
}