#include "common.h"
typedef struct { void *f0; void *f4; } SA;
void *func_800CD8A4(void *container, void *element);
s32 func_800CD090(void *container, void *element);
void *func_800D02F8(SA *p) {
    void *r = func_800CD8A4(p->f0, p->f4);
    if (func_800CD090(p->f0, p->f4) < 0) {
        p->f4 = 0;
    }
    return r;
}
