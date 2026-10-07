#include "common.h"

typedef struct {
    unsigned char pad0[0x20];
    u32 field_20;
} Obj800A80C0;

void func_800A80C0(Obj800A80C0 *obj, u32 value)
{
    obj->field_20 = value;
}
