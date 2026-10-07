#include "common.h"

/* Partial view: only the field cleared here is known. */
typedef struct {
    char pad0[0x20];
    u32 field_20;
} Obj;

void func_800A80D0(Obj *obj)
{
    obj->field_20 = 0;
}
