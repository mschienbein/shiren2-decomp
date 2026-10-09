#include "common.h"
typedef struct { unsigned char field_00[0x20]; s32 field_20; void *field_24; } Object;
extern s32 D_801535E8[];
extern void func_80136908(s32 *value);
extern s32 func_800A3934(Object *obj);
extern void func_800A3964(Object *obj);
Object *func_800A3850(Object *obj)
{
    obj->field_24 = D_801535E8;
    func_80136908(&obj->field_20);
    if (!func_800A3934(obj)) func_800A3964(obj);
    return obj;
}
