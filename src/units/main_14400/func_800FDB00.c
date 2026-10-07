#include "common.h"
typedef struct { char pad[0x24]; void *vtbl; } Obj;
extern char D_8015A880[];
extern void func_800EFD28(Obj *, s32);
extern void func_800A3918(Obj *);
void func_800FDB00(Obj *p, s32 flags) {
    p->vtbl = D_8015A880;
    func_800EFD28(p, 0);
    if (flags & 1) func_800A3918(p);
}
