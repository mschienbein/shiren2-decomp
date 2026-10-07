#include "common.h"

/* Returns the address of the member at offset 0xC. */
typedef struct {
    char pad0[0xC];
    u32 field_C;
} Obj;

u32 *func_800A7DDC(Obj *obj)
{
    return &obj->field_C;
}
