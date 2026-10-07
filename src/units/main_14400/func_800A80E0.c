#include "common.h"
typedef struct { unsigned char pad0[8]; signed char field_8; } Obj800A80E0;
void func_800A80E0(Obj800A80E0 *obj, s32 packed) { obj->field_8 = packed >> 24; }
