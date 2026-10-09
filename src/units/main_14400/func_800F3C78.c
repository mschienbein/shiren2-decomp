#include "common.h"
typedef unsigned short u16;
typedef struct { char pad0[0x7C]; u16 field7C; } Obj;
s32 func_800F3C78(Obj *obj) { return (obj->field7C >> 6) & 1; }
