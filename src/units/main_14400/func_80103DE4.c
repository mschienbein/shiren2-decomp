#include "common.h"

extern char D_8015BA78[];
typedef struct { char pad[0x24]; void *vtbl; } S;
void func_800EFD28(S *, s32);
void func_800A3918(void *);
void func_80103DE4(S *self, s32 flags) { self->vtbl = D_8015BA78; func_800EFD28(self, 0); if (flags & 1) func_800A3918(self); }
