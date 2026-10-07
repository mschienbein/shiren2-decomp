#include "common.h"

typedef struct { char pad[0x4C0]; s32 f4C0; } S;
s32 func_8008C89C(void*);
void func_8008D354(S *a, s32 b){ if (func_8008C89C((char*)a + b*32)) a->f4C0--;}
