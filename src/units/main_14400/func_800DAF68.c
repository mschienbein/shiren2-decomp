#include "common.h"
typedef struct { s32 pad; void *vtbl; } Obj;
extern char D_801583E8[];
extern void *func_800DA904(void *obj, s32 kind, unsigned char *params);
Obj *func_800DAF68(Obj *p, unsigned char *a) {
    func_800DA904(p, 13, a);
    p->vtbl = D_801583E8;
    return p;
}
