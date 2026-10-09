#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Vec;
typedef struct { Vec pos; char pad8[0x6D]; u8 state; } Obj;
extern s32 func_800E0F40(Obj *);
extern char *func_800A3B20(Obj *);
extern s32 func_800E115C(Obj *, s32, s32, u8, s32);
extern s32 func_80049CB4(s32, ...);
extern void func_800A7C1C(Obj *);
extern void func_800497F0(s32, ...);
extern s32 func_800A08D8(s32, s32, s32);
s32 func_800F426C(Obj *o, s32 kind, s32 what, u8 arg, s32 extra) {
    if (kind != 2 && what == 8) {
        s32 before;
        s32 after;
        char *h;
        before = (u8)func_800E0F40(o);
        h = func_800A3B20(o);
        func_800E115C(o, kind, 8, arg, extra);
        after = (u8)func_800E0F40(o);
        if (before != after) {
            Vec pos;
            Vec *pp = &pos;
            s32 x;
            s32 msg;
            pp->x = o->pos.x;
            pp->y = o->pos.y;
            x = func_80049CB4(0x109, pp);
            o->state = after;
            func_800A7C1C(o);
            msg = 0x197;
            if (kind == 0) msg = 0x28;
            func_800497F0(msg, x, h, func_800A3B20(o));
            func_800A08D8(1, x, 0);
        }
        return 1;
    }
    return func_800E115C(o, kind, what, arg, extra);
}
