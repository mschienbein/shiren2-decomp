#include "common.h"
typedef struct { char pad[0x2CC]; s32 field2CC; s32 field2D0; } Obj;
s32 func_8009A038(Obj *p) { s32 result = p->field2D0; if (result <= 0) result = p->field2CC != 0; return result; }
