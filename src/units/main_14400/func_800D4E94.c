#include "common.h"

typedef struct { char pad0[8]; signed char field_8; } Obj;

s32 func_800D4E94(Obj *obj) { return obj->field_8; }
