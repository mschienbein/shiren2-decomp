#include "common.h"
typedef struct { char pad[0x20]; short delta; short index; s32 (*fn)(void *); } VT;
typedef struct { s32 f0; VT *vt; } Obj;
typedef struct { unsigned char f0; unsigned char f1; char pad[0xA]; Obj sub; } SA;
s32 func_8009A364(SA *p) {
    Obj *o;
    VT *vt;
    if (p->f1 != 0xAC) {
        return 0;
    }
    o = &p->sub;
    vt = o->vt;
    return vt->fn((char *)o + vt->delta) != 0;
}
