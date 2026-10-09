#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xA0];
    s32 fieldA0;
} Obj80105304;

void func_80105304(Obj80105304 *obj)
{
    obj->fieldA0 = 1;
}
