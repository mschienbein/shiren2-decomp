#include "common.h"

void func_800DA9DC(unsigned char *, void *);
typedef struct { char x0; unsigned char x1; char pad[6]; char x8[8]; unsigned char x10; } S;
s32 func_800DD244(S *self, unsigned char *p) { *p++ = self->x1; func_800DA9DC(p++, self->x8); *p = self->x10; return 3; }
