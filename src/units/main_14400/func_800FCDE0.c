#include "common.h"

typedef struct { char pad[0x24]; void *f24; } S;
extern char D_8015A5F0[];
void func_800EFD28(S*, s32); void func_800A3918(S*);
void func_800FCDE0(S *a, s32 b){ a->f24 = D_8015A5F0; func_800EFD28(a, 0); if (b & 1) func_800A3918(a); }
