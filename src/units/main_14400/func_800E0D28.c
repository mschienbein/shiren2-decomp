#include "common.h"
typedef unsigned short u16;
typedef struct { s32 x, y; } Vec;
/* Slots +0x6C/+0x74 bind u32 func_800E0E88/func_800E96D4 and u32 func_800E0EAC/func_800E9750;
   the callers below narrow their results to u16 explicitly, as the original does (andi 0xFFFF). */
typedef struct { short delta; short index; u32 (*fn)(void *); } VEntry;
typedef struct { char pad[0x68]; VEntry getMax; VEntry getCur; } VTable;
typedef struct { Vec pos; char pad8[0x1C]; VTable *vtbl; char pad28[4]; u16 v2C; u16 v2E; } Obj;
extern s32 func_800A99D0(void);
extern s32 func_800E0AB4(void *, s32);
extern short func_800E0BD0(Obj *, s32);
extern s32 func_80049CB4(s32, ...);
extern char *func_800A3B20(void *);
extern void func_800497F0(s32, ...);
short func_800E0D28(Obj *o, short amt, short heal) {
    s32 n = 0;
    short ret;
    if (func_800A99D0() || amt < 0
        || (u16)o->vtbl->getMax.fn((char *)o + o->vtbl->getMax.delta)
           < (u16)o->vtbl->getCur.fn((char *)o + o->vtbl->getCur.delta)) n = 1;
    if (n) {
        ret = (short)func_800E0AB4(o, amt);
    } else {
        n = (u16)o->vtbl->getCur.fn((char *)o + o->vtbl->getCur.delta);
        func_800E0BD0(o, heal);
        o->v2C = o->v2E;
        n = (u16)o->vtbl->getCur.fn((char *)o + o->vtbl->getCur.delta) - n;
        if (n > 0) {
            Vec pos;
            Vec *pp = &pos;
            s32 r;
            pp->x = o->pos.x;
            pp->y = o->pos.y;
            r = func_80049CB4(0xDA, pp);
            func_800497F0(0x1F, r, func_800A3B20(o), n);
        }
        ret = n;
    }
    return ret;
}
