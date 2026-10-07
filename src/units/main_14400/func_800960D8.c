#include "common.h"

typedef unsigned char u8;

/* Partial view: only the field written here is known. */
typedef struct {
    char pad0[0x44];
    u8 field_44;
} Obj;

void func_800960D8(Obj *obj, u8 value)
{
    obj->field_44 = value;
}
