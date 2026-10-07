#include "common.h"
typedef struct { short delta; short index; void (*fn)(void *, s32, void *); } VEntry;
typedef struct { char pad[0x18]; VEntry e; } VTable;
typedef struct { char pad[0x18]; VTable *vtbl; } Obj;
typedef struct { char pad[0xC]; char body[4]; } Self;
extern void func_800AF11C(Self *, Obj *);
extern void func_800CA4A4(Obj *, void *);
extern char D_8015D2F4[];
void func_8010E180(Self *self, Obj *obj) {
    VTable *vt;
    func_800AF11C(self, obj);
    func_800CA4A4(obj, D_8015D2F4);
    vt = obj->vtbl;
    vt->e.fn((char *)obj + vt->e.delta, 4, self->body);
}
