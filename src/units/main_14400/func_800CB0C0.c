#include "common.h"
typedef struct VT { char pad[0x30]; short off; u32 (*fn)(void *, s32); } VT;
typedef struct { char pad[0x18]; VT *vt; } Obj;
typedef struct { char pad[0x18]; s32 x18; Obj *x1C; } S;
s32 func_800CB02C(s32 v);
void func_800CB0C0(S *p) {
    s32 a, b;
    if (p->x1C == 0) return;
    a = p->x1C->vt->fn((char *)p->x1C + p->x1C->vt->off, 0x27F8);
    b = p->x1C->vt->fn((char *)p->x1C + p->x1C->vt->off, 0x27FC);
    if (func_800CB02C(a) == b) p->x18 = a;
    else p->x18 = 0x22550FF;
}
