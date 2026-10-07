#include "common.h"

typedef struct { char pad0[0x18]; void *vtable; } S;
extern char D_8014A840[];
void *func_800CA030(S *s);
S *func_800442A4(S *s) {
    func_800CA030(s);
    s->vtable = D_8014A840;
    return s;
}
