#include "common.h"
typedef struct { s32 field_00[2]; void *field_08; } Object;
extern Object *func_801128F0(Object *obj, s32 type);
extern s32 D_80160640[];
Object *func_80127B0C(Object *obj)
{
    func_801128F0(obj, 0xED);
    obj->field_08 = D_80160640;
    return obj;
}
