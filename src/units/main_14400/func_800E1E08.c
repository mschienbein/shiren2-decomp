#include "common.h"
typedef struct { char pad[0x42]; unsigned char field_42; } Obj;
s32 func_800E1E08(Obj *p) { return p->field_42 != 0; }
