#include "common.h"

typedef struct {
    unsigned char pad0[7];
    signed char field_7;
    unsigned char field_8;
    unsigned char pad9[0x57];
    unsigned char field_60;
} Obj;

void func_80094DA0(Obj *obj);

Obj *func_80092600(Obj *obj)
{
    obj->field_8 = 0;
    obj->field_60 = 0;
    obj->field_7 = -1;
    func_80094DA0(obj);
    return obj;
}
