#include "common.h"

typedef struct Obj Obj;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj *func_80123230(Obj *object);

Obj *func_80123268(void)
{
    return func_80123230(func_800AC5B4(0x2C, 0));
}
