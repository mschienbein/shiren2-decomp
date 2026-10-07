#include "common.h"
typedef struct { s32 pad[2]; s32 field_8; } Inner;
typedef struct { Inner *field_0; } Obj;
s32 func_800D06FC(Obj *a) { return a->field_0->field_8; }
