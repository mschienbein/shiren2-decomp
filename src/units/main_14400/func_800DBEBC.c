#include "common.h"

typedef unsigned char u8;

typedef struct { u8 b0; u8 b1; u8 pad[6]; u8 x8[8]; u8 x10[8]; } S;
void func_800DA9DC(u8 *, void *);
s32 func_800DBEBC(S *s, u8 *out) { *out++ = s->b1; func_800DA9DC(out++, s->x8); func_800DA9DC(out, s->x10); return 3; }
