#include "common.h"
typedef struct { short field_00; short field_02; const void *field_04; } Object;
extern s32 D_80157FA8[];
extern const unsigned char D_80158AD8[48];
Object *func_800DF8C0(Object *obj)
{
    obj->field_04 = D_80157FA8;
    obj->field_00 = 1;
    obj->field_04 = D_80158AD8;
    return obj;
}
