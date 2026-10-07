#include "common.h"
typedef struct { char pad[4]; void *f4; } SA;
extern char D_80158688[];
void *func_800DA8A0(void *obj, s32 kind, void *src);
SA *func_800DC9E0(SA *p, void *src) {
    func_800DA8A0(p, 0x1B, src);
    p->f4 = D_80158688;
    return p;
}
