#include "common.h"
typedef struct { short delta; short index; s32 (*fn)(void *, void *, s32); } VtEntry;
typedef struct { unsigned b0 : 8; unsigned locked : 1; unsigned rest : 23; } Flags;
typedef struct { char pad[0x20]; Flags flags; } Player;
typedef struct { char pad0[4]; VtEntry *vt; } Base;
typedef struct { Base *base; void *arg; } Ref;
typedef struct { char pad[8]; Ref ref; } Obj;
extern Player *D_801476B8;
s32 func_800A692C(Player *, s32);
char *func_800AE674(void *);
void func_800498E4(s32, ...);
void func_80045A24(s32);
void func_800D0348(Ref *);
static inline s32 Flags_locked(Flags *f){ return f->locked; }
s32 func_800DD9AC(Obj *o){
    Ref *r = &o->ref;
    Base *b = r->base;
    s32 failed = b->vt[12].fn((char *)b + b->vt[12].delta, r->arg, 1) ^ 1;
    Player *pl;
    Flags f;
    s32 busy;
    if (failed) return 0;
    pl = D_801476B8;
    f = pl->flags;
    busy = !Flags_locked(&f) && func_800A692C(pl, 0x12);
    if (busy) {
        func_800498E4(0x113);
        return 0;
    }
    func_800498E4(0x7F, func_800AE674(r->arg));
    func_80045A24(0xE);
    func_800D0348(r);
    return 0;
}
