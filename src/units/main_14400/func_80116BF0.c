#include "common.h"
typedef struct { unsigned char pad_00[0xC]; unsigned char flags_0C; } Obj;
s32 func_80116BF0(Obj *obj) { return (obj->flags_0C >> 3) & 1; }
