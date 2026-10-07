#include "common.h"
typedef struct { char pad[0x4C]; void *unk4C; char pad50[0x10]; void *unk60; char pad64[0xC]; s32 unk70; } S;
extern char D_80152580[];
extern char D_80151E10[];
S *func_800953C0(S *);
S *func_8009A894(S *s) { S *r = s; func_800953C0(s); r->unk4C = D_80152580; r->unk60 = D_80151E10; r->unk70 = -1; return r; }
