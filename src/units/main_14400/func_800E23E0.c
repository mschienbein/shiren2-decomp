#include "common.h"

typedef struct { unsigned char pad00[0x54]; unsigned char flags54; } Obj;

void func_800E23E0(Obj *obj)
{
    obj->flags54 |= 4;
}
