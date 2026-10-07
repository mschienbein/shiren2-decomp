#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 x, y; } Vec2;
/* Entry of the target's table at +8; only its +0x3C message slot is called here. */
typedef struct { s16 delta; s16 index; s32 (*func)(void *self, void *event); } VEntry;
/* Entry of the unit table at +0x24; only its +0x94 slot is called here. */
typedef struct { s16 delta; s16 index; s32 (*func)(void *self, s32 a, s32 b, u8 c, s32 d); } UnitEntry;
typedef struct { u8 x0; u8 x1; } Thing;
typedef struct Obj {
    Vec2 pos;
    u8 pad8[0x14];
    u16 x1C;
    u8 pad1E[6];
    UnitEntry *vtbl;
    u8 pad28[0x20];
    Vec2 x48;
} Obj;
typedef struct { u8 pad[8]; VEntry *vtbl; } Target;
typedef struct { s32 type; s32 x4; Obj *src; u8 padC[0xC]; s32 x18; s32 pad1C; } Event;
typedef struct { u8 pad[0x10]; u32 flags; } Msg;
extern u32 D_8013960C;
extern s32 func_800E1CD4(Obj *, s32);
extern Thing *func_800B4D80(Obj *);
extern s32 func_800A251C(Vec2 *, Vec2 *);
extern Target *func_800E219C(Obj *, Vec2 *);
extern void func_800A578C(Obj *);
s32 func_800E5A78(Obj *o, Msg *msg){
    Vec2 pos;
    Vec2 *pp;
    u8 flags;
    u32 raw;
    if (o->x1C & 1) return 0;
    D_8013960C <<= 1;
    if (func_800E1CD4(o, 0x10)) {
        Thing *t = func_800B4D80(o);
        if (t == 0 || t->x1 != 0xDB) {
            o->vtbl[18].func((u8 *)o + o->vtbl[18].delta, 1, 0x10, 0, 0);
        }
    }
    pp = &pos;
    pp->x = o->pos.x;
    pp->y = o->pos.y;
    D_8013960C >>= 1;
    if (func_800A251C(pp, &o->x48)) return 0;
    raw = msg->flags;
    flags = raw;
    if (raw & 1) {
        Target *tgt = func_800E219C(o, pp);
        if (tgt) {
            Event ev;
            ev.type = 0x16;
            ev.x4 = 0;
            ev.src = o;
            ev.x18 = 0;
            /* message slot +0x3C: s32 handler(receiver, event); the result is not needed here */
            tgt->vtbl[7].func((u8 *)tgt + tgt->vtbl[7].delta, &ev);
        }
    }
    if (flags & 2) func_800A578C(o);
    o->x48 = pos;
    return 1;
}
