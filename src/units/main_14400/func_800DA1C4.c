#include "common.h"

typedef struct { unsigned short field_00; const void *field_04; unsigned short field_08; } Obj;
extern const unsigned char D_80157FA8[];
extern const unsigned char D_801581E8[];
Obj *func_800DA1C4(Obj *obj, unsigned char *value)
{
    obj->field_04 = D_80157FA8;
    obj->field_00 = 0x3C;
    obj->field_04 = D_801581E8;
    obj->field_08 = *value;
    return obj;
}
