#include "common.h"
typedef short s16;
typedef struct { char pad[0x14]; s16 field14; } Obj;
extern void func_800950A8(Obj *, void *);
void func_80095064(Obj *p, s32 value, void *descriptor) { p->field14 = value; func_800950A8(p, descriptor); }
