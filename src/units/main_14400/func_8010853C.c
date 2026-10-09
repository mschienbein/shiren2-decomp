#include "common.h"

typedef struct { unsigned char pad_00[0xC0]; s32 field_C0; } Obj;
void func_8010853C(Obj *obj)
{
    obj->field_C0 = 1;
}
