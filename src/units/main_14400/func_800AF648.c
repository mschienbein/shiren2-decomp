#include "common.h"
typedef struct { char pad[5]; signed char field_5; } Obj;
s32 func_800AF648(Obj *a) { return a->field_5 != -1; }
