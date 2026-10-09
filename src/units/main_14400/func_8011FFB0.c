#include "common.h"

typedef struct { s32 f0; s32 f4; void *f8; } S;
extern char D_8015F618[];
S *func_8010E290(S *, s32);
S *func_8011FFB0(S *s) {
    func_8010E290(s, 0xA3);
    s->f8 = D_8015F618;
    return s;
}
