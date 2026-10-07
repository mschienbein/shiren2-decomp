#include "common.h"

typedef struct { char pad0[0x8]; void *vtable; } S;
extern char D_8015E840[];
/* Returns the input object through this TU's partial view. */
S *func_80112D20(S *s, s32 kind);
S *func_8011B7F0(S *s) {
    func_80112D20(s, 0x23);
    s->vtable = D_8015E840;
    return s;
}
