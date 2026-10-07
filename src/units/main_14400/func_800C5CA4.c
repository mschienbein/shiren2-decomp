#include "common.h"
typedef struct { char pad[0x10]; s32 x10; } S;
void func_800C55B0(s32 *p);
s32 func_800C5CA4(S *p) { func_800C55B0(&p->x10); return p->x10; }
