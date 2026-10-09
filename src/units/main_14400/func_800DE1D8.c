#include "common.h"

typedef unsigned char u8;
typedef struct { short f0; short pad; void *f4; } S;
extern char D_80158958[];
S *func_800DDAD0(S*, s32); void func_800DDC0C(S*, u8*, s32);
S *func_800DE1D8(S *a, u8 *b){ s32 n; func_800DDAD0(a, 0x29); a->f4 = D_80158958; n = *b++; func_800DDC0C(a, b, n - 1); return a; }
