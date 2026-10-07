#include "common.h"
typedef struct { char pad[0x24]; void *vtbl; } Obj;
extern char D_80158C98[];
extern void func_800A3850(Obj *);
extern s32 func_800A3934(Obj *);
extern void func_800E01F0(Obj *);
Obj *func_800E0120(Obj *p) {
    func_800A3850(p);
    p->vtbl = D_80158C98;
    if (func_800A3934(p) == 0) func_800E01F0(p);
    return p;
}
