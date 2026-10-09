#include "common.h"
typedef struct { unsigned char pad_0[0x89]; unsigned char field_89; } Object;
extern void func_800E0528(Object *, s32);
void func_800FDA04(Object *object) { func_800E0528(object, object->field_89); }
