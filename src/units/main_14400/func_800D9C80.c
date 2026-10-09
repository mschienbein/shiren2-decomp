#include "common.h"

typedef struct VTable VTable;
typedef struct { unsigned short field00; VTable *vtable04; void *field08; } Obj;
extern VTable D_80157FA8;
extern VTable D_801580F8;

Obj *func_800D9C80(Obj *obj, void *value)
{
    obj->vtable04 = &D_80157FA8;
    obj->field00 = 11;
    obj->vtable04 = &D_801580F8;
    obj->field08 = value;
    return obj;
}
