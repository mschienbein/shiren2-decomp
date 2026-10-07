#include "common.h"

typedef struct { s32 f0; s32 f4; void *f8; } S;
extern char D_8015FE28[];
S *func_80115690(S *, s32);
S *func_801241D0(S *s) {
    func_80115690(s, 0xD4);
    s->f8 = D_8015FE28;
    return s;
}
