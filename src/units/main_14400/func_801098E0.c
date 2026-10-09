#include "common.h"
typedef struct Obj { char pad0[0x24]; void *field24; char pad28[0x8C]; void *fieldB4; } Obj;
extern unsigned char D_80159130[], D_80159150[], D_8015C788[];
s32 func_800EE598(Obj *obj);
void func_800E016C(Obj *obj, s32 flags);
void func_800A3918(Obj *obj);
void func_801098E0(Obj *obj, s32 flags) { obj->fieldB4 = D_80159130; obj->field24 = D_8015C788; func_800EE598(obj); obj->fieldB4 = D_80159130; obj->field24 = D_80159150; func_800E016C(obj, 0); if (flags & 1) func_800A3918(obj); }
