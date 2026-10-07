#include "common.h"
extern s32 D_8015AEE8[];
typedef struct { char pad[0x24]; s32 *field_24; } Obj;
extern void func_800EFD28(Obj *, s32), func_800A3918(Obj *);
void func_800FFC10(Obj *a, s32 b) { a->field_24 = D_8015AEE8; func_800EFD28(a, 0); if (b & 1) func_800A3918(a); }
