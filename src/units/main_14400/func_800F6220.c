#include "common.h"
typedef struct { unsigned char pad_0[0x94]; s32 field_94; } S;
extern void func_800F6038(S *, s32);
void func_800F6220(S *obj) { obj->field_94 = 1; func_800F6038(obj, 1); }
