#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x61];
    u8 field_61;
} Obj80092638;

void func_80092638(Obj80092638 *obj, u8 value)
{
    obj->field_61 = value;
}
