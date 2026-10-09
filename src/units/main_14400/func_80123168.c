#include "common.h"

typedef struct Object Object;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Object *func_80123130(Object *object);

Object *func_80123168(void)
{
    return func_80123130(func_800AC5B4(0x2C, 0));
}
