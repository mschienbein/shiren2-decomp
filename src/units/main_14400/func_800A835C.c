#include "common.h"

typedef unsigned short u16;

typedef struct {
    unsigned char pad0[0x1C];
    u16 field_1C;
} Obj800A835C;

void func_800A835C(Obj800A835C *obj, u16 value)
{
    obj->field_1C = value;
}
