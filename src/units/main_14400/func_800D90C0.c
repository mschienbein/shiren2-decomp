#include "common.h"

typedef short s16;

typedef struct {
    s16 kind;
    void *data;
} Obj;

extern char D_80157FA8[];
extern char D_80157FD8[];

Obj *func_800D90C0(Obj *obj)
{
    obj->data = D_80157FA8;
    obj->kind = 5;
    obj->data = D_80157FD8;
    return obj;
}
