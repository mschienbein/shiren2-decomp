#include "common.h"
typedef struct { s32 unk0; s32 unk4; void *vtbl8; short unkC; } Obj;
extern char D_8015D240[];
Obj *func_800AC0C0(Obj *, s32, s32);
Obj *func_8010DD70(Obj *o, s32 a1){ Obj *r; func_800AC0C0(o, 0xF, a1); r = o; r->vtbl8 = D_8015D240; r->unkC = 0; return r; }
