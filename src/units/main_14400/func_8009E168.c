#include "common.h"

typedef struct { char pad[0x90]; s32 count90; } S;
void func_8009DB9C(S *p, s32 *position);
void func_8009E168(S *p, s32 *position) { if (p->count90 > 0) func_8009DB9C(p, position); }
