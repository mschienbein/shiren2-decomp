#include "common.h"
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct Unit Unit;
typedef struct Owner Owner;
extern u16 D_80156A6E;
extern void *func_800B4928(Pos *pos);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800C94D8(void);
extern u16 func_80115944(void *owner, u16 value);
extern void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
static inline Pos *Pos_set(Pos *d, Pos *s) { d->x = s->x; d->y = s->y; return d; }
static inline s32 InRange(s32 v, s32 end) { return v < end; }
/* Trap slot +0x44: s32 (self, actor, source position, effect position, direction,
 * target unit, item). This handler reads self, actor and the effect position. */
s32 func_801259A0(Owner *self, void *actor, void *source_position, Pos *center,
                  void *direction, void *target_unit, void *item) {
    Pos p;
    Unit *u, *here;
    s32 top, right, bottom;
    {
        Pos *pp = Pos_set(&p, center);
        here = func_800B4928(pp);
        func_80049CB4(0xF4, pp);
    }
    if (here) {
        func_80049CB4(0x41, here);
        func_80049CB4(0x94, here);
    }
    p.y--;
    top = p.y;
    right = p.x + 2;
    bottom = top + 3;
    for (p.x = p.x - 1; InRange(p.x, right); p.x++) {
        for (p.y = top; InRange(p.y, bottom); p.y++) {
            s32 blocked = func_800C94D8() ^ 1;
            if (blocked) break;
            u = func_800B4928(&p);
            if (u) func_800A7B18(u, actor, func_80115944(self, D_80156A6E), 0xD);
        }
    }
    return 1;
}
