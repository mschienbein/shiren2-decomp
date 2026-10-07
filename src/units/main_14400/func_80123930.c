#include "common.h"

typedef struct { char pad0[0x8]; void *vtable; } S;
extern char D_801490B0[];
void func_80115570(S *s, s32 kind);
S *func_80123930(S *s) {
    func_80115570(s, 0xCB);
    s->vtable = D_801490B0;
    return s;
}
