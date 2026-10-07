#include "common.h"

typedef struct { char pad0[0x8]; void *vtable; } S;
extern char D_8015DFF8[];
S *func_80116D50(S *s, s32 kind);
S *func_801186E0(S *s) {
    func_80116D50(s, 14);
    s->vtable = D_8015DFF8;
    return s;
}
