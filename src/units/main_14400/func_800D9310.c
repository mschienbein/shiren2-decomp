#include "common.h"
typedef unsigned short u16;
typedef struct { short delta; short index; void *fn; } VtEntry;
typedef struct Actor { char pad[0x1C]; u16 flags1C; unsigned char state1E; char pad1F[5]; VtEntry *vt; char pad28[0xDC]; s32 unk104; } Actor;
typedef struct { s32 kind; Actor *who; s32 arg8; s32 argC; s32 arg10; s32 arg14; } Message;
extern Actor *D_801476B8;
Actor *func_800A6D40(Actor *);
void func_800E20F0(Actor *);
s32 func_80046240(void);
s32 func_800A44F4(Actor *, Actor *);
s32 func_800D9310(void *self /* receiver: unused; supplied by the vtable +0x14 call */){
    Actor *p = D_801476B8;
    Actor *t;
    s32 blocked = 0;
    s32 wasSet, r, result;
    Message msg;
    if (p->unk104 || ((s32 (*)(void *, s32, s32, unsigned char, s32))p->vt[18].fn)((char *)p + p->vt[18].delta, 2, 9, 0, 0)) blocked = 1;
    if (blocked) return 1;
    t = func_800A6D40(D_801476B8);
    if (t == 0) return 1;
    wasSet = t->flags1C & 0x200;
    t->flags1C |= 0x200;
    wasSet = wasSet != 0;
    if (t->state1E & 0x7C) {
        ((s32 (*)(void *, s32, s32, unsigned char, s32))t->vt[18].fn)((char *)t + t->vt[18].delta, 1, 0xC, 0, 0);
        func_800E20F0(t);
    }
    msg.kind = 0xD;
    {
        Message *m = &msg;
        m->who = D_801476B8;
        r = ((s32 (*)(void *, Message *))t->vt[11].fn)((char *)t + t->vt[11].delta, m);
    }
    if (!wasSet) t->flags1C &= ~0x200;
    result = 0;
    if (!r || func_80046240() || func_800A44F4(D_801476B8, t) == 1) result = 1;
    return result;
}
