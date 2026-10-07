#include "common.h"
typedef struct { s32 field_0; short field_4; unsigned char field_6[0xE]; s32 field_14; } Object;
extern void *func_8007946C(s32 group, s32 index);
extern void func_80079560(s32,s32,s32);
void func_800881C0(Object *arg) { func_8007946C(4,arg->field_14); func_80079560(4,arg->field_14,0); arg->field_4=4; }
