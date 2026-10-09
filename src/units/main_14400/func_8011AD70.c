#include "common.h"

typedef struct { s32 x; s32 y; } Pos;
typedef struct { s32 w[8]; } Iter;
typedef struct { s32 w[4]; } Area;
typedef struct { short delta; short index; s32 (*fn)(void *, s32, s32, unsigned char, s32); } VEntry;
typedef struct { char pad[0x24]; VEntry *vtbl; } Unit;
extern u32 D_8013960C;
extern u32 func_800B1C6C(void *pos);
extern s32 func_80049CB4(s32 id, ...);
/* Returns its rectangle by value through the hidden result pointer. */
extern Area func_800B3080(Pos *pos);
extern Iter *func_800A9204(Iter *, Area *, Pos *);
extern s32 func_800A9284(Iter *, s32);
extern Unit *func_800A942C(Iter *);
extern s32 func_800A58B8(Unit *);
extern s32 func_800A692C(Unit *, s32);
extern void func_800498E4(s32, ...);
static inline s32 is_target(Unit *u) {
    s32 ok = 0;
    if (!(func_800B1C6C(u) & 0x4000)) ok = func_800A58B8(u) != 1;
    return ok;
}

/* Item-effect slot +0x44 supplies self, actor and item; self and item are unused here. */
void func_8011AD70(void *arg0, Pos *center, void *item) {
    Pos pos;
    Iter it;
    Area area;
    Unit *unit;
    Unit *target;
    s32 eligible;
    s32 hit = 0;
    Pos *p = &pos;
    p->x = center->x;
    D_8013960C *= 2;
    p->y = center->y;
    func_80049CB4(0x76, center, (s32)(func_800B1C6C(p) & 0x1000) > 0);
    area = func_800B3080(p);
    func_800A9204(&it, &area, center);
    while (func_800A9284(&it, 0x7C)) {
        unit = func_800A942C(&it);
        target = unit;
        if (!is_target(unit)) continue;
        eligible = func_800A692C(target, 10) != 1;
        if (eligible) {
            func_80049CB4(6);
            target->vtbl[18].fn((char *)target + target->vtbl[18].delta, 0, 11, 0xFE, 0);
            hit = 1;
            func_80049CB4(7);
        }
    }
    D_8013960C >>= 1;
    if (!hit) func_800498E4(0x222);
}
