#include "common.h"
typedef unsigned char u8;

extern char D_801587D8[];
void *func_800DA904(void *, s32, u8 *);
typedef struct { s32 x0; void *vtbl; } S;
S *func_800DD448(S *self, u8 *params) { func_800DA904(self, 0x22, params); self->vtbl = D_801587D8; return self; }
