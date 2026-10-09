#include "common.h"
typedef struct S { char pad0[0x24]; void *field24; } S;
typedef struct Obj800A38A0 Obj800A38A0;
extern unsigned char D_8015C5D0[];
void func_800EFD28(S *, s32);
void func_800A3918(Obj800A38A0 *obj);
void func_80109178(S *obj, s32 flags) { obj->field24 = D_8015C5D0; func_800EFD28(obj, 0); if (flags & 1) func_800A3918((void *)obj); }
