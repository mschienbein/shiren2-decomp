#include "common.h"

typedef struct {
    unsigned char padC0[0xC4];
    s32 field_C4;
} Obj;

void func_80108550(Obj *obj)
{
    obj->field_C4 = 10;
}
