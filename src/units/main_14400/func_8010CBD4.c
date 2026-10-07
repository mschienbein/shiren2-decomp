#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s16 delta; s16 index; void (*fn)(); } VEntry;
typedef struct { u8 pad[0x18]; VEntry *vtbl; } Obj;
extern u8 D_8015D0CC[];
void func_800AF174(u8 *, Obj *);
void func_800CA4E8(Obj *, void *);
void func_8010CBD4(u8 *s, Obj *o) {
    VEntry *e;
    func_800AF174(s, o);
    func_800CA4E8(o, D_8015D0CC);
    e = &o->vtbl[5];
    ((void (*)(void *, s32, void *))e->fn)((u8 *)o + e->delta, 2, s + 0xC);
}
