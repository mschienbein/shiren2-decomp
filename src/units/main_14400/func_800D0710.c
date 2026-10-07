#include "common.h"
typedef struct { char pad[0x10]; short field_10; s32 (*field_14)(void *); } VTable;
typedef struct { void *field_0; VTable *field_4; } Obj;
extern s32 func_800AF920(void *, unsigned char);
s32 func_800D0710(Obj *a) { s32 count = 0; s32 i = a->field_4->field_14((char *)a + a->field_4->field_10) - 1; for (; i >= 0; i--) { if (func_800AF920(a->field_0, i)) count++; } return count; }
