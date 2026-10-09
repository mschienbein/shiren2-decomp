#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 field0;
    u8 field1;
} Obj800DB918;

s32 func_800DB918(Obj800DB918 *obj, u8 *out)
{
    *out = obj->field1;
    return 1;
}
