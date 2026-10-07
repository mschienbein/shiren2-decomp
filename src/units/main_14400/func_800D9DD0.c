#include "common.h"
typedef struct { short id; short pad; void *vtbl; } Obj;
extern char D_80157FA8[];
extern char D_80158128[];
Obj *func_800D9DD0(Obj *p) {
    p->vtbl = D_80157FA8;
    p->id = 0x31;
    p->vtbl = D_80158128;
    return p;
}
