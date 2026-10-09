#include "common.h"

typedef short s16;

typedef struct {
    s16 kind00;
    void **vtable04;
    void *target08;
} Obj800D9AD0;

extern void *D_80157FA8[];
extern void *D_801580C8[];

Obj800D9AD0 *func_800D9AD0(Obj800D9AD0 *obj, void *target)
{
    obj->vtable04 = D_80157FA8;
    obj->kind00 = 10;
    obj->vtable04 = D_801580C8;
    obj->target08 = target;
    return obj;
}
