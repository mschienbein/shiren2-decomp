#include "common.h"

/* Partial view of the animated record through its float at 0x184. */
typedef struct {
    unsigned char pad0[0x184];
    float field_184;
} Obj80090BE8;

void func_80090BE8(Obj80090BE8 *obj, float value)
{
    obj->field_184 = value;
}
