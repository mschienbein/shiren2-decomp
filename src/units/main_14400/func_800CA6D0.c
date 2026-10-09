#include "common.h"
typedef struct { char pad0[0x18]; void *field18; } Obj;
extern unsigned char D_80149F80[];
void func_800D8FA8(void *object);
void func_800CA6D0(Obj *obj, s32 flags) { obj->field18 = D_80149F80; if (flags & 1) func_800D8FA8(obj); }
