#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
typedef struct VT { char pad[0x90]; short off; s32 (*fn)(void *, s32, s32, u8, s32); } VT;
typedef struct { char pad[9]; unsigned char x9; char pad2[0x1A]; VT *vt; } Ent;
typedef struct { s32 w[4]; } Area;
typedef struct { s32 w[8]; } Iter;
void *func_800A2FD0(Area *a, Pair *pos, u8 radius);
Iter *func_800A9204(Iter *it, Area *a, Pair *pos);
s32 func_800A9284(Iter *it, s32 mask);
s32 func_80049CB4(s32 id, ...);
Ent *func_800A942C(Iter *it);
s32 func_800A692C(Ent *e, s32 kind);
u32 func_800B1C6C(Ent *e);
void func_800498E4(s32 message_id, ...);
/* Item-effect slot +0x44 supplies self, actor and item; self and item are unused here. */
void func_8011D050(void *unused, Pair *pos, void *item) {
    Area area;
    Iter it;
    Pair tmp;
    Ent *e;
    s32 hit = 0;
    s32 ok;
    s32 res;
    func_800A2FD0(&area, pos, 1);
    func_800A9204(&it, &area, pos);
    while (func_800A9284(&it, 0x7C)) {
        if (!hit) {
            tmp.x = pos->x;
            tmp.y = pos->y;
            func_80049CB4(0x11D, &tmp);
        }
        e = func_800A942C(&it);
        if ((res = func_800A692C(e, 10) ^ 1) == 0) continue;
        ok = (e->x9 & 0xF) == 2 || !(func_800B1C6C(e) & 0x80);
        if (!ok) continue;
        e->vt->fn((char *)e + e->vt->off, 0, 4, 0xFE, 0);
        hit = 1;
    }
    if (!hit) {
        func_80049CB4(0x132);
        func_800498E4(0x222);
    }
}
