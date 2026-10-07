#include "common.h"

typedef struct { s32 f0; s32 f4; void *f8; } S;
extern char D_8015E9D0[];
/* Returns the input object through this TU's partial view. */
S *func_80112D20(S *, s32);
S *func_8011C140(S *s) {
    func_80112D20(s, 0x28);
    s->f8 = D_8015E9D0;
    return s;
}
