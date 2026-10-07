#include "common.h"
typedef struct { s32 pad; void *vtbl; } Obj;
extern char D_80157FA8[];
extern void func_800D8FE8(void *);
void func_800DFE9C(Obj *p, s32 flags) {
    p->vtbl = D_80157FA8;
    if (flags & 1) func_800D8FE8(p);
}
