#include "common.h"
typedef struct { char pad[8]; short field_8; void (*field_C)(void *, s32); } VTable;
typedef struct { char pad[8]; VTable *field_8; } Obj;
extern s32 func_800AF28C(Obj *, s32 *);
s32 func_80127CC0(Obj *a, s32 *b) { if (*b == 0x1B) { if (a) a->field_8->field_C((char *)a + a->field_8->field_8, 3); return 1; } else return func_800AF28C(a, b); }
