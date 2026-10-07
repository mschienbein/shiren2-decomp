#include "common.h"

typedef struct { char pad0[0x8]; void *vtable; } S;
extern char D_8015EE60[];
/* Returns the input object through this TU's partial view. */
S *func_80111530(S *s, s32 kind);
S *func_8011DFB0(S *s) {
    func_80111530(s, 0x8F);
    s->vtable = D_8015EE60;
    return s;
}
