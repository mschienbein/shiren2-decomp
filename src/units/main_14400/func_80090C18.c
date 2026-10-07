#include "common.h"

/* Partial view of the animated record through its float at 0x19C. */
typedef struct {
    unsigned char pad0[0x19C];
    float field_19C;
} Obj80090C18;

void func_80090C18(Obj80090C18 *obj, float value)
{
    obj->field_19C = value;
}
