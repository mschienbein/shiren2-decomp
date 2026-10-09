#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[9]; u8 field_9; } Obj;
extern void func_801E8F78(u8 value);
s32 func_800DA18C(Obj *obj) { func_801E8F78(obj->field_9); return 0; }
