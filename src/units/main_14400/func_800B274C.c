#include "common.h"
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { Pos first; Pos last; } Rect;
typedef struct { Pos cur; Pos first; Pos last; } Iter;
extern Rect D_801429C0;
extern u16 D_80143450[][76];
extern Pos *func_800A3610(Pos *out, Iter *it);
extern u32 func_800B1C6C(Pos *pos);
extern void func_800B2884(void);
static __inline__ void set_start(Iter *it, s32 x, s32 y) {
    Pos p;
    p.x = x; p.y = y;
    it->first = p;
    it->cur = it->first;
}
static __inline__ void set_end(Iter *it, s32 x, s32 y) {
    Pos p;
    p.x = x; p.y = y;
    it->last = p;
}
static __inline__ s32 valid(Iter *it) { return it->cur.x <= it->last.x; }
s32 func_800B274C(s32 kind) {
    Iter it;
    s32 changed = 0;
    set_start(&it, D_801429C0.first.x, D_801429C0.first.y);
    set_end(&it, D_801429C0.last.x, D_801429C0.last.y);
    while (valid(&it)) {
        Pos pos;
        func_800A3610(&pos, &it);
        if (func_800B1C6C(&pos) & 0x2000) {
            D_80143450[pos.x][pos.y] = kind | (D_80143450[pos.x][pos.y] & 0xDFFF);
            changed = 1;
        }
    }
    if (changed) func_800B2884();
    return changed;
}
