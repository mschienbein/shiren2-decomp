#include "common.h"
typedef struct { unsigned char pad_00[0x1E]; unsigned char flags_1E; } Obj;
s32 func_800A7D9C(Obj *obj) { return (obj->flags_1E >> 2) & 1; }
