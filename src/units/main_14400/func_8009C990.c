#include "common.h"

typedef struct {
    unsigned char pad0[0x168];
    u32 field_168;
} Obj8009C990;

void func_8009C990(Obj8009C990 *obj, u32 value)
{
    obj->field_168 = value;
}
