#include "common.h"

/* Partial view: only the field cleared here is known. */
typedef struct {
    char pad0[0x128];
    u32 field_128;
} Obj;

void func_8009FBB0(Obj *obj)
{
    obj->field_128 = 0;
}
