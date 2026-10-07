#include "common.h"
typedef struct { char pad[0xA]; unsigned char unkA; char padB[0x1E - 0xB]; unsigned char unk1E; } Unit;
typedef struct { s32 cur; s32 data[7]; } UnitIter;
extern char D_80147620[];
void func_800A919C(UnitIter *, void *pos);
s32 func_800A9284(UnitIter *, s32);
Unit *func_800A942C(UnitIter *);
s32 func_800E1CC4(Unit *, s32);
unsigned char func_800C57CC(void *, unsigned char);
static inline s32 skip(Unit *u, unsigned char team) {
    s32 r = 0;
    if (u->unkA == team) {
        if (u->unk1E & 3) r = 1;
        else if (func_800E1CC4(u, 1) == 0) r = 1;
    }
    return r;
}
void *func_800B31E8(void *pos, s32 team) {
    unsigned short t = team;
    UnitIter it;
    Unit *last;
    s32 n;
    Unit *u;
    UnitIter *p;
    func_800A919C(&it, pos);
    last = 0;
    n = 0;
    for (p = &it; func_800A9284(p, 0xFF); ) {
        u = func_800A942C(p);
        if (skip(u, t)) {
            last = u;
            n++;
        }
    }
    if (n < 2) return last;
    n = func_800C57CC(D_80147620, n - 1);
    it.cur = 0;
    for (p = &it; func_800A9284(p, 0xFF); ) {
        u = func_800A942C(p);
        if (skip(u, t)) {
            if (--n == -1) return u;
        }
    }
    return 0;
}
