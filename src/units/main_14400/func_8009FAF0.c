#include "common.h"

typedef struct {
    unsigned char pad0[0x124];
    u32 field_124;
} Obj8009FAF0;

void func_8009FAF0(Obj8009FAF0 *obj, u32 value)
{
    obj->field_124 = value;
}
