#include "common.h"
typedef struct { unsigned char pad_0[0x54]; unsigned char field_54; } Obj;
s32 func_800E2410(Obj *obj) { unsigned char flags = obj->field_54 & 2; return flags != 0; }
