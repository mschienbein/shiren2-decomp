#include "common.h"

typedef struct { s32 f0; s32 f4; void *f8; } S;
extern char D_8015E100[];
S *func_80116D50(S *, s32);
S *func_80118BD0(S *s) {
    func_80116D50(s, 0x11);
    s->f8 = D_8015E100;
    return s;
}
