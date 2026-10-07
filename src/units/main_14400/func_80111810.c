#include "common.h"

/* g++ 2.x vtable entry: this-adjust delta, index, function pointer. */
typedef struct { short delta; short index; s32 (*fn)(); } VEntry;

extern s32 D_8015D5A4;
typedef struct { char pad[0x18]; VEntry *vt18; } Obj;
typedef struct { char pad[0xC]; s32 xC; } S;
void func_800AF174(S *p, Obj *o);
void func_800CA4E8(Obj *o, s32 *desc);
void func_80111810(S *p, Obj *o) {
    func_800AF174(p, o);
    func_800CA4E8(o, &D_8015D5A4);
    o->vt18[5].fn((char *)o + o->vt18[5].delta, 2, &p->xC);
}
