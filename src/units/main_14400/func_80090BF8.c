#include "common.h"

/* Partial view of the animated record through its float at 0x18C. */
typedef struct {
    char pad0[0x18C];
    float field_18C;
} Obj;

void func_80090BF8(Obj *obj, float value)
{
    obj->field_18C = value;
}
