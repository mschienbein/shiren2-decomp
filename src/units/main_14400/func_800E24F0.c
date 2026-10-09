#include "common.h"
typedef struct { unsigned char pad_0[0x64]; u32 field_64; u32 field_68; } Obj;
s32 func_800E24F0(Obj *obj) { return (obj->field_68 | obj->field_64) != 0; }
