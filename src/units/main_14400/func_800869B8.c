#include "common.h"
typedef struct { char pad0[4]; short state; char pad6[0xE]; s32 handle; } S;
extern void func_8007D320(s32);
void func_800869B8(S *p) { func_8007D320(p->handle); p->state = 4; }
