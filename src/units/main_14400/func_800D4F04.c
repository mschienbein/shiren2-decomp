#include "common.h"
typedef struct { char pad0[8]; signed char field8; char pad9[3]; void *fieldC; } Obj;
void func_800D4F04(void *arg0) { Obj *obj = arg0; obj->field8 = -1; obj->fieldC = 0; }
