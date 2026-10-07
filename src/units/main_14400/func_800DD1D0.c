#include "common.h"
typedef unsigned char u8;
typedef struct { s32 type; char pad4[0x14]; s32 arg; char pad1C[4]; } Msg;
typedef struct { short delta; short index; void (*fn)(void *, Msg *); } VEntry;
typedef struct { char pad[0x38]; VEntry e; } VTable;
typedef struct { char pad[8]; VTable *vtbl; } Obj;
typedef struct { char pad[0xC]; Obj *target; u8 arg; } Self;
extern void *D_801476B8;
extern void *func_800EB9FC(void *);
extern void func_800498E4(s32, ...);
s32 func_800DD1D0(Self *self) {
    if (func_800EB9FC(D_801476B8)) {
        func_800498E4(0xB7);
    } else {
        Msg m;
        Obj *obj;
        VTable *vt;
        m.arg = self->arg;
        m.type = 0x20;
        obj = self->target;
        vt = obj->vtbl;
        vt->e.fn((char *)obj + vt->e.delta, &m);
    }
    return 1;
}
