#include "common.h"

typedef struct { s32 f0; s32 f4; void *f8; } S;
extern char D_8015EFA8[];
/* Returns the input object through this TU's partial view. */
S *func_80111530(S *, s32);
S *func_8011E6C0(S *s) {
    func_80111530(s, 0x92);
    s->f8 = D_8015EFA8;
    return s;
}
