#include "common.h"

typedef struct { char pad0[0x4C]; void *vtable; } S;
extern char D_80152E00[];
S *func_800953C0(S *s);
S *func_8009EDA0(S *s) {
    func_800953C0(s);
    s->vtable = D_80152E00;
    return s;
}
