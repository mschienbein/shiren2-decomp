#include "common.h"
typedef struct { unsigned char field_00[4]; short field_04; unsigned char field_06[8]; short field_0E; s32 field_10; s32 field_14; } Object;
extern void func_8004194C(s32 id, void *outY, void *outX);
extern void func_80061820(s32, s32);
void func_80086AC8(Object *object) { s32 a, b; func_8004194C(object->field_14, &a, &b); func_80061820(a, b); object->field_0E = 1; object->field_04 = 4; }
