#include "common.h"
typedef struct { char pad[0x18]; short delta; short index; void (*fn)(void *, s32, void *); } VT;
typedef struct { char pad[0x18]; VT *vt; } Obj;
extern char D_8015D9CC[];
void func_800AF11C(void *a, Obj *b);
void func_800CA4A4(Obj *b, void *name);
void func_801163B0(void *a, Obj *b) {
    VT *vt;
    func_800AF11C(a, b);
    func_800CA4A4(b, D_8015D9CC);
    vt = b->vt;
    vt->fn((char *)b + vt->delta, 1, (char *)a + 0xC);
}
