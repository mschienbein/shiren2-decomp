#include "common.h"

/* g++ 2.x vtable entry: this-adjust delta, index, function pointer. */
typedef struct { short delta; short index; void *(*fn)(void *, u32); } VEntry;

typedef struct { s32 x0; VEntry *vt4; } Obj;
typedef struct { char pad[0x8C]; Obj *obj8C; } S;
void func_800CD304(Obj *o, u32 arg);
void *func_800F1750(S *p) {
    Obj *o = p->obj8C;
    if (o != 0) {
        void *r = o->vt4[7].fn((char *)o + o->vt4[7].delta, 0);
        func_800CD304(p->obj8C, 0);
        return r;
    }
    return 0;
}
