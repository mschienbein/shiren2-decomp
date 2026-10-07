#include "common.h"

/* g++ 2.x vtable entry: this-adjust delta, index, function pointer. */
typedef struct { short delta; short index; s32 (*fn)(void *, void *); } VEntry;

typedef unsigned char u8;
typedef struct { char pad[8]; u8 x8; } Unit;
typedef struct { char pad[0x24]; VEntry *vt24; } Obj;
typedef struct { s32 kind; Unit *unit4; s32 pad8[2]; s32 value10; u8 *extra14; } Msg;
/* Whole RNG object (state pointers plus backing words); only its base is passed here. */
extern u8 D_80147620[];
extern s32 D_801483C0[][4];
s32 func_800F1040(Unit *u, Obj *o, s32 id);
s32 func_800E0F40(Unit *u);
u8 func_800C57A0(void *rng);
void func_800E20F0(Obj *o);
s32 func_80049CB4(s32 id, ...);
s32 func_800E20CC(void *u);
char *func_800A3B20(void *u);
void func_800498E4(s32 id, ...);
void func_800E3678(Obj *o, Unit *u);
s32 func_800FC7EC(Unit *u, Obj *o) {
    Msg msg;
    Msg *mp;
    u8 extra;
    s32 r, n, sel;
    r = func_800F1040(u, o, 0x51);
    switch (r) {
    case 2:
        return 1;
    case 1:
        return 0;
    }
    r = (u8)func_800E0F40(u);
    switch (r) {
    case 1:
        n = 1;
        sel = 0;
        break;
    case 2:
        n = 3;
        sel = func_800C57A0(D_80147620) & 1;
        break;
    default:
        n = 4;
        sel = func_800C57A0(D_80147620) & 3;
        break;
    }
    func_800E20F0(o);
    extra = u->x8;
    func_80049CB4(0x51, u);
    func_80049CB4(6);
    func_80049CB4(0x23, o, 0, 0);
    func_80049CB4(7);
    func_80049CB4(0x1131);
    mp = &msg;
    while (1) {
        if (!(n-- > 0)) break;
        {
            u8 *ep = &extra;
            s32 value = D_801483C0[sel][n];
            msg.kind = 0x13;
            mp->unit4 = u;
            mp->extra14 = ep;
            mp->value10 = value;
        }
        if (o->vt24[11].fn((char *)o + o->vt24[11].delta, mp)) {
            func_80049CB4(0x132);
            func_800E3678(o, u);
            return 1;
        }
    }
    if (func_800E20CC(u)) func_800498E4(0x57, func_800A3B20(u));
    func_800E3678(o, u);
    return 1;
}
