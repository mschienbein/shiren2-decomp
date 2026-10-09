#include "common.h"
typedef struct S S;
extern void *func_800AC5B4(s32, s32);
extern S *func_80123930(S *);
S *func_80123968(void) { return func_80123930(func_800AC5B4(0x10, 0)); }
