#include "common.h"

extern char D_801C5428[];
extern char D_00194FC0[];
void func_8006AC30(void *, void *, void *, s32, s32, s32);
typedef struct { s32 x0; void *x4; } S;
char *func_800AC064(S *s) { func_8006AC30(D_801C5428, D_00194FC0, s->x4, 2, 0, 0x3E); return D_801C5428; }
