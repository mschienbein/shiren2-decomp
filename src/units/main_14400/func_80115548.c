#include "common.h"

typedef struct { unsigned char pad00[0x28]; unsigned char field28; } Obj;

s32 func_80115548(Obj *obj)
{
    return obj->field28 != 0;
}
