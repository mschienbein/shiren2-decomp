#include "common.h"

typedef unsigned short u16;

/* Partial view: only the field written here is known. */
typedef struct {
    char pad0[0x408];
    u16 field_408;
} Obj;

void func_800479A8(Obj *obj, u16 value)
{
    obj->field_408 = value;
}
